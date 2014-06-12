#include "input/joystick.h"
#include "core/channel.h"
#include "opengl.h"
#include <cassert>

namespace overdrive {
	namespace input {
		Joystick::Joystick(int id, float deadZone) :
			mJoystickID(id),
			mDeadZone(deadZone)
		{
			int numButtons = 0;
			int numAxes = 0;

			auto buttons = glfwGetJoystickButtons(mJoystickID, &numButtons);
			auto axes = glfwGetJoystickAxes(mJoystickID, &numAxes);

			if (id >= 0)
				mJoystickName = glfwGetJoystickName(mJoystickID);

			if (numButtons > 0) {
				mButtonStates.reserve(numButtons);

				std::copy(
					buttons,
					buttons + numButtons,
					std::back_inserter(mButtonStates)
				);
			}

			if (numAxes > 0) {
				mAxisStates.reserve(numAxes);
				
				std::copy(
					axes,
					axes + numAxes,
					std::back_inserter(mAxisStates)
				);
			}
		}

		void Joystick::update() {
			auto oldAxes = mAxisStates;
			auto oldButtons = mButtonStates;

			mAxisStates.clear();
			mButtonStates.clear();

			int numAxes;
			int numButtons;

			auto newAxes = glfwGetJoystickAxes(mJoystickID, &numAxes);
			auto newButtons = glfwGetJoystickButtons(mJoystickID, &numButtons);

			if (numButtons > 0)
				std::copy(
					newButtons,
					newButtons + numButtons,
					std::back_inserter(mButtonStates)
				);

			if (numButtons > 0)
				std::copy(
					newAxes,
					newAxes + numAxes,
					std::back_inserter(mAxisStates)
				);

			for (int i = 0; i < numAxes; ++i) {
				if (std::abs(oldAxes[i] - newAxes[i]) > mDeadZone) {
					OnMove mv;
					
					mv.mJoystickID = mJoystickID;
					mv.mPosition = mAxisStates;
					mv.mDelta.resize(numAxes);
					
					for (int j = 0; j < numAxes; ++j)
						mv.mDelta[j] = mAxisStates[j] - oldAxes[j];

					core::Channel::broadcast(mv);

					break; // only send one move message per update
				}
			}

			for (int i = 0; i < numButtons; ++i) {
				if (mButtonStates[i] != oldButtons[i]) {
					if (isPressed(i))
						core::Channel::broadcast(OnButtonPress{ mJoystickID, i });
					else
						core::Channel::broadcast(OnButtonRelease{ mJoystickID, i });
				}
			}
		}

		int Joystick::getJoystickID() const {
			return mJoystickID;
		}

		const std::string& Joystick::getJoystickName() const {
			return mJoystickName;
		}

		int Joystick::getNumButtons() const {
			return mButtonStates.size();
		}

		int Joystick::getNumAxes() const {
			return mButtonStates.size();
		}

		void Joystick::setDeadZone(float value) {
			mDeadZone = value;
		}

		float Joystick::getDeadZone() const {
			return mDeadZone;
		}
		
		float Joystick::getAxis(int index) const {
			assert(static_cast<size_t>(index) <mAxisStates.size());
			assert(index >= 0);

			return mAxisStates[index];
		}

		const std::vector<float>& Joystick::getAxes() const {
			return mAxisStates;
		}

		bool Joystick::isPressed(int buttonIndex) const {
			assert(static_cast<size_t>(buttonIndex) < mButtonStates.size());
			assert(buttonIndex >= 0);
			
			return (mButtonStates[buttonIndex] == GLFW_PRESS);
		}

		unsigned char Joystick::getButton(int buttonIndex) const {
			assert(static_cast<size_t>(buttonIndex) < mButtonStates.size());
			assert(buttonIndex >= 0);

			return mButtonStates[buttonIndex];
		}

		const std::vector<unsigned char>& Joystick::getButtons() const {
			return mButtonStates;
		}
	}
}
