#include "input/keyboard.h"
#include "core/channel.h"
#include "core/logger.h"
#include "video/window.h"

#include <cassert>
#include <unordered_map>

namespace {
	static std::unordered_map<
		const overdrive::video::Window*, 
		overdrive::input::Keyboard*
	> mKeyboardRegistry;

	void onKey(
		GLFWwindow* handle, 
		int keyCode, 
		int , //scancode
		int action, 
		int modifiers
	) {
		using namespace overdrive;

		if (auto w = video::Window::getFromHandle(handle)) {
			auto it = mKeyboardRegistry.find(w);
			assert(it != mKeyboardRegistry.end());
			it->second->setButtonState(keyCode, (action == GLFW_PRESS));

			switch (action) {
			case GLFW_PRESS:
				core::Channel::broadcast(input::Keyboard::OnKeyPress{ keyCode, modifiers, w });
				break;

			case GLFW_RELEASE:
				core::Channel::broadcast(input::Keyboard::OnKeyRelease{ keyCode, modifiers, w });
				break;
			}
		}
		else
			gLog.warning() << "received key from unregistered window";
	}
}

namespace overdrive {
	namespace input {
		Keyboard::Keyboard(const video::Window* associatedWindow) {
			for (auto& key : mKeyState)
				key = false;

			registerKeyboard(associatedWindow, this);

			glfwSetKeyCallback(associatedWindow->getHandle(), &onKey);
		}

		bool Keyboard::operator[](int button) const {
			assert(button >= 0);
			assert(button < GLFW_KEY_LAST);

			return mKeyState[button];
		}

		void Keyboard::setButtonState(int button, bool pressed) {
			assert(button >= 0);
			assert(button < GLFW_KEY_LAST);

			mKeyState[button] = pressed;
		}

		bool Keyboard::isAssociatedWith(const video::Window* window) const {
			return (window == mAssociatedWindow);
		}

		void registerKeyboard(const video::Window* win, Keyboard* kb) {
			assert(mKeyboardRegistry.find(win) == mKeyboardRegistry.end());
			mKeyboardRegistry[win] = kb;
		}

		void unregisterKeyboard(const video::Window* win) {
			assert(mKeyboardRegistry.find(win) != mKeyboardRegistry.end());
			mKeyboardRegistry.erase(win);
		}
	}
}