#include "stdafx.h"
#include "../preprocessor.h"

#if OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_WINDOWS

#include <Xinput.h>
#include <cassert>
#include <cmath>
#include <unordered_map>
#include <boost/optional.hpp>
#include "../core/channel.h"
#include "../util/exception_windows.h"
#include "../math/clamp.h"

#include "gamepad.h"

namespace overdrive {
	namespace input {
		namespace {
			// look in xinput.h for descriptions of parameters

			HMODULE gXInputLib = nullptr;
			int gNumGamepads = 0;

			typedef DWORD(WINAPI *_XInputGetState)(DWORD, XINPUT_STATE*);
			typedef DWORD(WINAPI *_XInputSetState)(DWORD, XINPUT_VIBRATION*);
			typedef DWORD(WINAPI *_XInputGetCapabilities)(DWORD, DWORD, XINPUT_CAPABILITIES*);
			typedef DWORD(WINAPI *_XInputEnable)(BOOL);
			typedef DWORD(WINAPI *_XInputGetAudioDeviceIds)(DWORD, LPWSTR, UINT*, LPWSTR, UINT*);
			typedef DWORD(WINAPI *_XInputGetBatteryInformation)(DWORD, BYTE, XINPUT_BATTERY_INFORMATION*);
			typedef DWORD(WINAPI *_XInputGetKeystroke)(DWORD, DWORD, PXINPUT_KEYSTROKE);

			_XInputGetState					xInputGetState = nullptr;
			_XInputSetState					xInputSetState = nullptr;
			_XInputGetCapabilities			xInputGetCapabilities = nullptr;
			_XInputEnable					xInputEnable = nullptr;
			_XInputGetAudioDeviceIds		xInputGetAudioDeviceIds = nullptr;
			_XInputGetBatteryInformation	xInputGetBatteryInformation = nullptr;
			_XInputGetKeystroke				xInputGetKeystroke = nullptr;

			float gamepadStick(SHORT value_, float deadzone) {
				using std::abs;

				float result = 0.0f;
				float value = static_cast<float>(value_) / 32767.0f;
				
				// if the value falls into the deadzone, yield 0
				if (abs(value) < deadzone)
					return 0.0f;

				// otherwise compensate and try to rescale the area outside of the deadzone
				// into a [-1..1] range
				if (value > deadzone)
					result = value - deadzone;
				else
					result = value + deadzone;
				
				return result / (1.0f - deadzone);
			}

			float gamepadTrigger(BYTE value_, float threshold) {
				float value = static_cast<float>(value_) / 255.0f;
				
				if (value < threshold)
					return 0.0f;
				else
					return (value - threshold) / (1.0f - threshold);
			}
		}
		
		namespace detail {
			void loadXInput() {
				gXInputLib = ::LoadLibraryA(XINPUT_DLL_A);

				if (gXInputLib) {
					xInputGetState = (_XInputGetState) ::GetProcAddress(gXInputLib, "XInputGetState");
					xInputSetState = (_XInputSetState) ::GetProcAddress(gXInputLib, "XInputSetState");
					xInputGetCapabilities = (_XInputGetCapabilities) ::GetProcAddress(gXInputLib, "XInputGetCapabilities");
					xInputEnable = (_XInputEnable) ::GetProcAddress(gXInputLib, "XInputEnable");
					xInputGetAudioDeviceIds = (_XInputGetAudioDeviceIds) ::GetProcAddress(gXInputLib, "XInputGetAudioDeviceIds");
					xInputGetBatteryInformation = (_XInputGetBatteryInformation) ::GetProcAddress(gXInputLib, "XInputGetBatteryInformation");
					xInputGetKeystroke = (_XInputGetKeystroke) ::GetProcAddress(gXInputLib, "XInputGetKeystroke");
				}
				else
					throw util::WinException();
			}

			void unloadXInput() {
				if (gXInputLib) {
					xInputGetState = nullptr;
					xInputSetState = nullptr;
					xInputGetCapabilities = nullptr;
					xInputEnable = nullptr;
					xInputGetAudioDeviceIds = nullptr;
					xInputGetBatteryInformation = nullptr;
					xInputGetKeystroke = nullptr;

					::FreeLibrary(gXInputLib);
				}
			}
		}

		unsigned int Gamepad::getMaxGamepads() {
			return XUSER_MAX_COUNT;
		}

		void Gamepad::setVibration(float left_, float right_) {
			assert(xInputSetState);

			float left = clamp(left_, 0.0f, 1.0f);
			float right = clamp(right_, 0.0f, 1.0f);

			XINPUT_VIBRATION vib = {
				static_cast<WORD>(left * 65535),
				static_cast<WORD>(right * 65535)
			};

			xInputSetState(mID, &vib);

			mLeftVibration = left;
			mRightVibration = right;
		}

		void Gamepad::setDefaultDeadzones() {
			mLeftDeadzone = static_cast<float>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / 32767.0f;
			mRightDeadzone = static_cast<float>(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) / 32767.0f;
			mTriggerThreshold = static_cast<float>(XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / 256.0f;
		}

		boost::optional<Gamepad::State> Gamepad::getGamepadState(unsigned int id) {
			using core::Channel;

			XINPUT_STATE fullstate;

			assert(xInputGetState); // make sure that xinput was loaded and the getstate function was set
			assert(id < XUSER_MAX_COUNT); // <- apparantly this is 4

			static unsigned long long tryTimes[XUSER_MAX_COUNT] = {};
			static DWORD lastPacket[XUSER_MAX_COUNT] = {};

			auto currentTick = GetTickCount64();

			if (
				(tryTimes[id] == 0) ||	// if either the gamepad wasn't tried or it was ver
				(currentTick >= tryTimes[id])
			) {
				DWORD res = xInputGetState(id, &fullstate);

				if (res == ERROR_SUCCESS) {
					if (fullstate.dwPacketNumber != lastPacket[id]) {
						if (!mIsConnected) {
							mIsConnected = true;
							tryTimes[id] = 0;
							Channel::broadcast(OnConnected{ this }); // notify on connections
						}

						lastPacket[id] = fullstate.dwPacketNumber; // store the packet number

						// convert xinput state
						State result;

						result.mButtonState = fullstate.Gamepad.wButtons;
						result.mLeftStickX = gamepadStick(fullstate.Gamepad.sThumbLX, mLeftDeadzone);
						result.mLeftStickY = gamepadStick(fullstate.Gamepad.sThumbLY, mLeftDeadzone);
						result.mRightStickX = gamepadStick(fullstate.Gamepad.sThumbRX, mRightDeadzone);
						result.mRightStickY = gamepadStick(fullstate.Gamepad.sThumbRY, mRightDeadzone);
						result.mLeftTrigger = gamepadTrigger(fullstate.Gamepad.bLeftTrigger, mTriggerThreshold);
						result.mRightTrigger = gamepadTrigger(fullstate.Gamepad.bRightTrigger, mTriggerThreshold);

						return result;
					}
				}
				else if (res == ERROR_DEVICE_NOT_CONNECTED) {
					if (mIsConnected) {
						mIsConnected = false;
						Channel::broadcast(OnDisconnected{ this }); // notify on disconnect
					}

					tryTimes[id] = currentTick + 5000; // try again in around 5 seconds
				}
			}

			return boost::none; 
		}
	}
}

#endif
