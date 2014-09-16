#include "input/keyboard.h"
#include "core/logger.h"
#include <algorithm>

namespace overdrive {
	namespace input {
		Keyboard::Keyboard(GLFWwindow* handle) :
			mHandle(handle)
		{
			assert(mHandle != nullptr);

			std::uninitialized_fill(
				std::begin(mKeyState),
				std::end(mKeyState),
				false
			);
		}

		bool Keyboard::operator[](int button) const {
			assert(button > 0);
			assert(button < GLFW_KEY_LAST);
			return mKeyState[button];
		}

		GLFWwindow* Keyboard::getHandle() const {
			return mHandle;
		}
	}
}
