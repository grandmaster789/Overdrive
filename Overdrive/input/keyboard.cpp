#include "keyboard.h"
#include "../video/window.h"
#include "../core/channel.h"

namespace {
	/*
	void unicodeCallback(GLFWwindow* handle, unsigned int codepoint) {
	}

	void unicodeModsCallback(GLFWwindow* handle, unsigned int codepoint, int modifiers) {
	}
	*/

	void keyCallback(
		GLFWwindow* handle, 
		int key, 
		int, // scancode
		int action, 
		int modifiers
	) {
		using overdrive::video::Window;
		using overdrive::input::Keyboard;
		using overdrive::core::Channel;

		Window* w = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		Keyboard* kbd = w->getKeyboard();

		switch (action) {
		case GLFW_PRESS:
			kbd->mKeyState[key] = true;
			Channel::broadcast(Keyboard::OnKeyPress{ kbd, key, modifiers });
			break;

		case GLFW_RELEASE:
			kbd->mKeyState[key] = false;
			Channel::broadcast(Keyboard::OnKeyRelease{ kbd, key, modifiers });
			break;
		
			// just ignore anything other than press and release
		}
	}
}

namespace overdrive {
	namespace input {
		Keyboard::Keyboard(video::Window* sourceWindow):
			mSourceWindow(sourceWindow)
		{
			if (sourceWindow) {
				std::uninitialized_fill(
					std::begin(mKeyState),
					std::end(mKeyState),
					false
				);

				/*
				glfwSetCharCallback(sourceWindow->getHandle(), unicodeCallback);
				glfwSetCharModsCallback(sourceWindow->getHandle(), unicodeModsCallback);
				*/
				glfwSetKeyCallback(sourceWindow->getHandle(), keyCallback);

			}
		}

		void Keyboard::setStickyKeys(bool enabled) {
			glfwSetInputMode(mSourceWindow->getHandle(), GLFW_STICKY_KEYS, enabled ? GL_TRUE : GL_FALSE);
		}

		bool Keyboard::getStickyKeys() const {
			return (glfwGetInputMode(mSourceWindow->getHandle(), GLFW_STICKY_KEYS) != GL_FALSE);
		}

		video::Window* Keyboard::getSourceWindow() const {
			return mSourceWindow;
		}

		bool Keyboard::operator[](int button) const {
			assert(button > 0);
			assert(button < GLFW_KEY_LAST);
			return mKeyState[button];
		}
	}
}