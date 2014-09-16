#include "input/joystick.h"
#include "core/channel.h"
#include "opengl.h"

namespace overdrive {
	namespace input {
		Joystick::Joystick(int id, float deadzone):
			mJoystickID(id),
			mDeadZone(deadzone)
		{
			int numAxes = 0;
			int numButtons = 0;

			if (id != NOT_PRESENT) {
				auto buttons = glfwGetJoystickButtons(mJoystickID, &numButtons);
				auto axes = glfwGetJoystickAxes(mJoystickID, &numAxes);
				mJoystickName = glfwGetJoystickName(mJoystickID);

				for (int i = 0; i < numButtons; ++i) {
					switch (buttons[i]) {
					case GLFW_PRESS:
						mButtonState.push_back(eButtonState::PRESSED);
						break;

					case GLFW_REPEAT:
						mButtonState.push_back(eButtonState::REPEAT);
						break;

					case GLFW_RELEASE:
					default:
						mButtonState.push_back(eButtonState::RELEASED);
						break;
					}
				}

				for (int i = 0; i < numAxes; ++i)
					mAxisState.push_back(axes[i]);
			}
			else
				mJoystickName = "Not present";
		}

		void Joystick::poll() {
			// note that this assumes the number of axes and buttons does not change for a given joystick
			if (mJoystickID == NOT_PRESENT)
				return;

			auto oldButtons = mButtonState;
			auto oldAxes = mAxisState;

			int numAxes = 0;
			int numButtons = 0;

			auto newButtons = glfwGetJoystickButtons(mJoystickID, &numButtons);
			auto newAxes = glfwGetJoystickAxes(mJoystickID, &numAxes);

			assert(numAxes == static_cast<int>(mAxisState.size()));
			assert(numButtons == static_cast<int>(mButtonState.size()));

			for (int i = 0; i < numButtons; ++i) {
				switch (newButtons[i]) {
				case GLFW_PRESS:
					mButtonState[i] = eButtonState::PRESSED;
					if (mButtonState[i] != oldButtons[i])
						core::Channel::broadcast(OnButtonPress{ i, this });
					break;

				case GLFW_REPEAT:
					mButtonState[i] = eButtonState::REPEAT;
					// Repeat shouldn't cause continous messages
					break;

				case GLFW_RELEASE:
				default:
					mButtonState[i] = eButtonState::RELEASED;
					if (mButtonState[i] != oldButtons[i])
						core::Channel::broadcast(OnButtonRelease{ i, this });
					break;
				}
			}

			for (int i = 0; i < numAxes; ++i)
				mAxisState[i] = newAxes[i];

			for (int i = 0; i < numAxes; ++i) {
				if (std::abs(mAxisState[i] - oldAxes[i]) > mDeadZone) {
					OnMove message;

					message.mPosition = mAxisState;
					
					message.mDelta.resize(numAxes);
					for (int j = 0; j < numAxes; ++j)
						message.mDelta[j] = mAxisState[j] - oldAxes[j];

					message.mJoystick = this;

					overdrive::core::Channel::broadcast(message);

					break; // only send one move message per poll
				}
			}
		}

		int Joystick::getJoystickID() const {
			return mJoystickID;
		}

		const std::string& Joystick::getName() const {
			return mJoystickName;
		}

		void Joystick::setDeadZone(float deadzone) {
			mDeadZone = deadzone;
		}

		float Joystick::getDeadZone() const {
			return mDeadZone;
		}

		int Joystick::getNumButtons() const {
			return static_cast<int>(mButtonState.size());
		}

		int Joystick::getNumAxes() const {
			return static_cast<int>(mAxisState.size());
		}
		
		bool Joystick::isPressed(int buttonID) const {
			return (mButtonState[buttonID] != eButtonState::RELEASED);
		}

		bool Joystick::isRepeating(int buttonID) const {
			return (mButtonState[buttonID] == eButtonState::REPEAT);
		}

		bool Joystick::isReleased(int buttonID) const {
			return (mButtonState[buttonID] == eButtonState::RELEASED);
		}

		Joystick::eButtonState Joystick::getButton(int button) const {
			return mButtonState[button];
		}

		const std::vector<Joystick::eButtonState>& Joystick::getButtonState() const {
			return mButtonState;
		}
	}
}