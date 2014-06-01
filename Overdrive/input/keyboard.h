#ifndef OVERDRIVE_INPUT_KEYBOARD_H
#define OVERDRIVE_INPUT_KEYBOARD_H

#include "opengl.h"
#include "input/input.h"

namespace overdrive {
	namespace video {
		class Window;
	}

	namespace input {
		class Keyboard {
		public:
			Keyboard(const video::Window* associatedWindow);

			// For an overview of keycodes, see http://www.glfw.org/docs/latest/group__keys.html
			// An overview of modifiers can be found at http://www.glfw.org/docs/latest/group__mods.html

			struct OnKeyPress {
				int mKey;
				int mModifiers;
				video::Window* mAssociatedWindow;
			};

			struct OnKeyRelease {
				int mKey;
				int mModifiers;
				video::Window* mAssociatedWindow;
			};

			bool operator[](int button) const; // usage -- (keyboard[GLFW_BUTTON_ESCAPE] == true)

			void setButtonState(int button, bool pressed);
			
			bool isAssociatedWith(const video::Window* window) const;

		private:
			bool mKeyState[GLFW_KEY_LAST]; // if a key is pressed, the relevant flag turns true
			
			const video::Window* mAssociatedWindow;
		};

		void registerKeyboard(const video::Window* win, Keyboard* kb);
		void unregisterKeyboard(const video::Window* win); // this is handled by the Input subsystem
	}
}

#endif
