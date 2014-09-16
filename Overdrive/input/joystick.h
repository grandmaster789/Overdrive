#ifndef OVERDRIVE_INPUT_JOYSTICK_H
#define OVERDRIVE_INPUT_JOYSTICK_H

#include <vector>

namespace overdrive {
	namespace input {
		class Joystick {
		public:
			// Enums and typedefs
			enum eJoystickID {
				NOT_PRESENT = -1
			};

			enum class eButtonState {
				PRESSED,
				RELEASED,
				REPEAT
			};

			// Signals
			struct OnMove {
				std::vector<float> mPosition;
				std::vector<float> mDelta;
				Joystick* mJoystick;
			};

			struct OnButtonPress {
				int mButtonID;
				Joystick* mJoystick;
			};

			struct OnButtonRelease {
				int mButtonID;
				Joystick* mJoystick;
			};

			struct OnConnect{
				Joystick* mJoystick;
			};

			struct OnDisconnect {
				Joystick* mJoystick;
			};

			// Functions
			Joystick(int id = NOT_PRESENT, float deadzone = 0.1f); // the deadzone is a threshold value for broadcasting axis movement

			void poll(); // note that this assumes the number of axes and buttons does not change for a given joystick

			int getJoystickID() const;
			const std::string& getName() const;

			void setDeadZone(float deadzone);
			float getDeadZone() const;

			int getNumButtons() const;
			int getNumAxes() const;

			bool isPressed(int buttonID) const; // note that this will also return true if the actual button state is 'REPEAT'
			bool isRepeating(int buttonID) const;
			bool isReleased(int buttonID) const;
			eButtonState getButton(int buttonID) const;
			const std::vector<eButtonState>& getButtonState() const;

		private:
			int mJoystickID = NOT_PRESENT;
			std::string mJoystickName;

			std::vector<eButtonState> mButtonState;
			std::vector<float> mAxisState;

			float mDeadZone;
		};
	}
}

#endif
