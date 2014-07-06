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
			setJoystickID(id);
		}

		void Joystick::update() {
			std::vector<float> oldAxes;
			std::vector<unsigned char> oldButtons;

			std::swap(mAxisStates, oldAxes);
			std::swap(mButtonStates, oldButtons);
			
			mButtonStates.clear();
			mAxisStates.clear();

			int numAxes;
			int numButtons;

			auto newAxes = glfwGetJoystickAxes(mJoystickID, &numAxes);
			auto newButtons = glfwGetJoystickButtons(mJoystickID, &numButtons);

			if ((numButtons > 0) && (newButtons != nullptr))
				std::copy(
					newButtons,
					newButtons + numButtons - 1,
					std::back_inserter(mButtonStates)
				);

			if ((numAxes > 0) && (newAxes != nullptr))
				std::copy(
					newAxes,
					newAxes + numAxes - 1,
					std::back_inserter(mAxisStates)
				);

			if (oldAxes.size() == static_cast<size_t>(numAxes)) {
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
			}

			if (oldButtons.size() == static_cast<size_t>(numButtons)) {
				for (int i = 0; i < numButtons; ++i) {
					if (mButtonStates[i] != oldButtons[i]) {
						if (isPressed(i))
							core::Channel::broadcast(OnButtonPress{ mJoystickID, i });
						else
							core::Channel::broadcast(OnButtonRelease{ mJoystickID, i });
					}
				}
			}
		}

		void Joystick::setJoystickID(int id) {
			mJoystickID = id;

			mButtonStates.clear();
			mAxisStates.clear();

			if (id >= 0)
				mJoystickName = glfwGetJoystickName(mJoystickID);
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
