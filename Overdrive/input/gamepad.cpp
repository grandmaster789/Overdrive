#include "gamepad.h"
#include "../core/channel.h"
#include <cmath>
#include <array>

namespace overdrive {
	namespace input {
		Gamepad::Gamepad(unsigned int id):
			mID(id)
		{
			using core::Channel;

			mState = {};
			
			setDefaultDeadzones();

			Channel::broadcast(OnConnected{ this });
		}

		Gamepad::~Gamepad() {
			setVibration(0, 0);
		}

		bool Gamepad::operator == (const Gamepad& other) const {
			return (
				(mState.mButtonState == other.mState.mButtonState) &&
				(mState.mLeftStickX == other.mState.mLeftStickX) &&
				(mState.mLeftStickY == other.mState.mLeftStickY) &&
				(mState.mRightStickX == other.mState.mRightStickX) &&
				(mState.mRightStickY == other.mState.mRightStickY) &&
				(mState.mLeftTrigger == other.mState.mLeftTrigger) &&
				(mState.mRightTrigger == other.mState.mRightTrigger)
			);
		}

		bool Gamepad::operator != (const Gamepad& other) const {
			return (
				(mState.mButtonState != other.mState.mButtonState) ||
				(mState.mLeftStickX != other.mState.mLeftStickX) ||
				(mState.mLeftStickY != other.mState.mLeftStickY) ||
				(mState.mRightStickX != other.mState.mRightStickX) ||
				(mState.mRightStickY != other.mState.mRightStickY) ||
				(mState.mLeftTrigger != other.mState.mLeftTrigger) ||
				(mState.mRightTrigger != other.mState.mRightTrigger)
			);
		}

		bool Gamepad::isConnected() const {
			return mIsConnected;
		}

		void Gamepad::update() {
			using std::swap;
			using std::abs;
			using std::make_pair;
			using core::Channel;

			auto optState = getGamepadState(mID);

			if (optState) {
				State state = *optState;

				swap(state, mState);

				if ((abs(mState.mLeftStickX - state.mLeftStickX) > mLeftDeadzone) ||
					(abs(mState.mLeftStickY - state.mLeftStickY) > mLeftDeadzone)
				)
					Channel::broadcast(OnLeftStickMoved{
						this,
						make_pair(mState.mLeftStickX, mState.mLeftStickY),
						make_pair(
							mState.mLeftStickX - state.mLeftStickX,
							mState.mLeftStickY - state.mLeftStickY
						)
					});

				if ((abs(mState.mRightStickX - state.mRightStickX) > mRightDeadzone) ||
					(abs(mState.mRightStickY - state.mRightStickY) > mRightDeadzone)
				)
					Channel::broadcast(OnRightStickMoved{
						this,
						make_pair(mState.mRightStickX, mState.mRightStickY),
						make_pair(
							mState.mRightStickX - state.mRightStickX,
							mState.mRightStickY - state.mRightStickY
						)
					});

				uint32_t currentState = mState.mButtonState;
				uint32_t oldState = state.mButtonState;

				uint32_t changed = currentState ^ oldState;
				uint32_t pressed = currentState & changed;
				uint32_t released = (~currentState) & changed;

				if (pressed)
					Channel::broadcast(OnButtonPressed{ this, pressed });

				if (released)
					Channel::broadcast(OnButtonReleased{ this, released });
			}
		}

		void Gamepad::setDeadzone(float left, float right) {
			assert(left < 1.0f);
			assert(left >= 0.0f);
			assert(right < 1.0f);
			assert(right >= 0.0f);

			mLeftDeadzone = left;
			mRightDeadzone = right;
		}

		std::pair<float, float> Gamepad::getDeadzone() const {
			return std::make_pair(mLeftDeadzone, mRightDeadzone);
		}

		uint32_t Gamepad::getButtonState() const {
			return mState.mButtonState;
		}

		std::pair<float, float> Gamepad::getLeftStick() const {
			return std::make_pair(mState.mLeftStickX, mState.mLeftStickY);
		}

		std::pair<float, float> Gamepad::getRightStick() const {
			return std::make_pair(mState.mRightStickX, mState.mRightStickY);
		}

		float Gamepad::getLeftTrigger() const {
			return mState.mLeftTrigger;
		}

		float Gamepad::getRightTrigger() const {
			return mState.mRightTrigger;
		}

		std::pair<float, float> Gamepad::getVibration() const {
			return std::make_pair(mLeftVibration, mRightVibration);
		}
	}
}