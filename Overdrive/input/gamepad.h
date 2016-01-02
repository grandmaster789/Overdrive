#pragma once

#include <cstdint>
#include <utility>
#include <boost/optional.hpp>

namespace overdrive {
	namespace input {
		/*
			Based around XInput state model.

			Note that the implementation is platform-specific
		 */
		class Gamepad {
		public:
			enum eButtonField {
				// playstation-style
				CROSS		= 0x1000,
				CIRCLE		= 0x2000,
				SQUARE		= 0x4000,
				TRIANGLE	= 0x8000,

				// xbox-style 
				A			= 0x1000,
				B			= 0x2000,
				X			= 0x4000,
				Y			= 0x8000,

				// dpad
				UP			= 0x0001,
				DOWN		= 0x0002,
				LEFT		= 0x0004,
				RIGHT		= 0x0008,

				// menu
				START		= 0x0010,
				BACK		= 0x0020,

				// stick presses
				LEFT_STICK  = 0x0040,
				RIGHT_STICK	= 0x0080,

				// shoulder
				LEFT_SHOULDER	= 0x0100,
				RIGHT_SHOULDER	= 0x0200
			};

			Gamepad(unsigned int id);
			~Gamepad();
			
			Gamepad(const Gamepad&) = delete;
			Gamepad(Gamepad&&) = default;
			Gamepad& operator = (const Gamepad&) = delete;
			Gamepad& operator = (Gamepad&&) = default;

			static unsigned int getMaxGamepads(); // platform specific ~> look in input_windows.cpp

			// comparison operations only compare the actual button state, none of the other stuff
			// [NOTE] the floats almost ensure that they will never equal... include deadzone?
			bool operator == (const Gamepad& other) const;
			bool operator != (const Gamepad& other) const;

			bool isConnected() const;

			void setDeadzone(float left, float right); // [0..1> are valid
			std::pair<float, float> getDeadzone() const; // yields [left, right] zones

			void update(); 

			uint32_t getButtonState() const; // use bit masks to obtain single button states

			std::pair<float, float> getLeftStick() const;
			std::pair<float, float> getRightStick() const;
			
			float getLeftTrigger() const;
			float getRightTrigger() const;

			// ----- Vibration -----
			void setVibration(float left, float right); // values are clamped to [0..1]
			std::pair<float, float> getVibration() const;

			// ----- Events -----
			struct OnLeftStickMoved { 
				Gamepad* mGamePad; 
				std::pair<float, float> mNewPosition;
				std::pair<float, float> mDelta;
			};

			struct OnRightStickMoved {
				Gamepad* mGamepad;
				std::pair<float, float> mNewPosition;
				std::pair<float, float> mDelta;
			};

			struct OnLeftTriggerMoved {
				Gamepad* mGamepad;
				float mNewTriggerPosition;
				float mDelta;
			};

			struct OnRightTriggerMoved {
				Gamepad* mGamepad;
				float mNewTriggerPosition;
				float mDelta;
			};

			struct OnButtonPressed { Gamepad* mGamepad; uint32_t mButtonState; };
			struct OnButtonReleased { Gamepad* mGamepad; uint32_t mButtonState; };

			struct OnConnected { Gamepad* mGamepad; };
			struct OnDisconnected { Gamepad* mGamepad; };

		private:
			void setDefaultDeadzones();

			unsigned int mID;

			float mLeftDeadzone;
			float mRightDeadzone;
			float mTriggerThreshold;

			float mLeftVibration;
			float mRightVibration;

			struct State {
				uint32_t mButtonState; // bitfield representing all of the buttons

				float mLeftStickX;		// [-1..1]
				float mLeftStickY;		// [-1..1]
				float mRightStickX;		// [-1..1]
				float mRightStickY;		// [-1..1]

				float mLeftTrigger;		// [0..1]
				float mRightTrigger;	// [0..1]
			} mState;

			bool mIsConnected;

			boost::optional<State> getGamepadState(unsigned int id); // platform specific implementation, look for it in input_windows.cpp
		};
	}
}
