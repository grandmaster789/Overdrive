#ifndef OVERDRIVE_INPUT_MOUSE_H
#define OVERDRIVE_INPUT_MOUSE_H

#include "opengl.h"

/*
	TODO: decide on sticky button states (see glfwSetInputMode documentation)
*/

namespace overdrive {
	namespace input {
		class Mouse {
		public:
			// Shorter names; coincidentally, the actual integer values range from 0-7
			enum eButton : int {
				BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT,
				BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
				BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
				BUTTON_1 = GLFW_MOUSE_BUTTON_1,
				BUTTON_2 = GLFW_MOUSE_BUTTON_2,
				BUTTON_3 = GLFW_MOUSE_BUTTON_3,
				BUTTON_4 = GLFW_MOUSE_BUTTON_4,
				BUTTON_5 = GLFW_MOUSE_BUTTON_5,
				BUTTON_6 = GLFW_MOUSE_BUTTON_6,
				BUTTON_7 = GLFW_MOUSE_BUTTON_7,
				BUTTON_8 = GLFW_MOUSE_BUTTON_8,
				BUTTON_LAST = GLFW_MOUSE_BUTTON_LAST
			};

			enum class eCursorState {
				NORMAL,		// visible cursor, behaving normally
				HIDDEN,		// hidden cursor, still behaving normally
				DISABLED	// hidden cursor, disabled window clipping (useful for 3d camera controls)
			};

			// An overview of modifiers can be found at http://www.glfw.org/docs/latest/group__mods.html

			// Signals
			struct OnButtonPress {
				eButton mButton;
				int mModifiers;
				Mouse* mMouse;
			};

			struct OnButtonRelease {
				eButton button;
				int mModifiers;
				Mouse* mMouse;
			};

			struct OnScroll {
				double mXOffset;
				double mYOffset;
				Mouse* mMouse;
			};

			struct OnEnter {
				Mouse* mMouse;
			};

			struct OnLeave {
				Mouse* mMouse;
			};

			struct OnMove {
				double mX;	// in screen coordinates
				double mY;	// in screen coordinates
				double mDeltaX;
				double mDeltaY;
				Mouse* mMouse;
			};

			// Functions
			Mouse(GLFWwindow* handle);

			void hideCursor();
			void disableCursor();
			void restoreCursor();
			void setCursorState(eCursorState state);
			eCursorState getCursorState() const;

			void setPosition(double x, double y); // the pointer will be moved to the indicated position, triggering an OnMouseMove
			void getPosition(double& x, double& y) const;

			void setInsideClientArea(bool isInside);
			bool isInsideClientArea() const;

			bool operator [] (eButton button) const; // usage -- bool pressed = (mouse[Mouse::MOUSE_BUTTON_LEFT]);

			bool mButtonState[BUTTON_LAST];
			double mX = 0;
			double mY = 0;

			GLFWwindow* getHandle() const;

		private:
			bool mIsInsideClientArea;

			GLFWwindow* mHandle;
		};
	}
}

#endif
