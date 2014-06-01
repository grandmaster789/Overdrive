#ifndef OVERDRIVE_INPUT_MOUSE_H
#define OVERDRIVE_INPUT_MOUSE_H

#include "opengl.h"
#include "core/channel.h"

namespace overdrive {
	namespace video {
		class Window;
	}

	namespace input {
		class Mouse {
		public:
			// Typedefs and enums
			enum eButton : int {
				MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT,
				MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
				MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
				MOUSE_BUTTON_1 = GLFW_MOUSE_BUTTON_1,
				MOUSE_BUTTON_2 = GLFW_MOUSE_BUTTON_2,
				MOUSE_BUTTON_3 = GLFW_MOUSE_BUTTON_3,
				MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4,
				MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5,
				MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6,
				MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7,
				MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8,
				MOUSE_BUTTON_LAST = GLFW_MOUSE_BUTTON_LAST
			};

			// Signals
			struct OnButtonPress {
				eButton mButton;
				int mModifiers;
				video::Window* mAssociatedWindow;
			};

			struct OnButtonRelease {
				eButton mButton;
				int mModifiers;
				video::Window* mAssociatedWindow;
			};

			struct OnScroll {
				double mXOffset;
				double mYOffset;
				video::Window* mAssociatedWindow;
			};

			struct OnEnter {
				video::Window* mAssociatedWindow;
			};

			struct OnLeave {
				video::Window* mAssociatedWindow;
			};

			struct OnMove {
				double mX;		// in screen coordinates
				double mY;		// in screen coordinates
				double mDeltaX;
				double mDeltaY;

				video::Window* mAssociatedWindow;
			};

			// Functions
			Mouse(const video::Window* associatedWindow);

			void setHidden(bool enable = true);
			bool getHidden() const;

			void setButtonState(eButton button, bool pressed);
			void setPosition(double x, double y, bool setCursor = true); // if setCursor is true, the pointer will be moved to the indicated position

			bool operator[](eButton button) const; // usage -- bool pressed = (mouse[Mouse::MOUSE_BUTTON_LEFT]);
			void getPosition(double& x, double& y) const;

			bool isAssociatedWith(const video::Window* window) const;

		private:
			bool mButtonState[MOUSE_BUTTON_LAST];

			double mX = 0;
			double mY = 0;

			bool mHidden = false;

			const video::Window* mAssociatedWindow;
		};

		void registerMouse(const video::Window* window, Mouse* mouse);
		void unregisterMouse(const video::Window* window); // this is handled by the Input system
	}
}

#endif
