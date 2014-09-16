#ifndef OVERDRIVE_INPUT_KEYBOARD_H
#define OVERDRIVE_INPUT_KEYBOARD_H

#include <boost/noncopyable.hpp>
#include "opengl.h" // for GLFW_KEY_LAST and GLFWwindow

namespace overdrive {
	namespace input {
		class Keyboard:
			boost::noncopyable
		{
		public:
			Keyboard(GLFWwindow* handle);

			// For an overview of keycodes, see http://www.glfw.org/docs/latest/group__keys.html
			// An overview of modifiers can be found at http://www.glfw.org/docs/latest/group__mods.html

			struct OnKeyPress {
				int mKey;
				int mModifiers;
				Keyboard* mKeyboard;
			};

			struct OnKeyRelease {
				int mKey;
				int mModifiers;
				Keyboard* mKeyboard;
			};

			bool operator[](int button) const; // ex: keyboard[GLFW_BUTTON_ESCAPE] == false
			GLFWwindow* getHandle() const;
			
			bool mKeyState[GLFW_KEY_LAST];

		private:
			GLFWwindow* mHandle;
		};
	}
}

#endif
