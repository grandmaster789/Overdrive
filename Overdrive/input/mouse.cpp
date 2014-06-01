#include "input/mouse.h"
#include "video/window.h"
#include "core/logger.h"

#include <cassert>
#include <unordered_map>

namespace {
	static std::unordered_map<
		const overdrive::video::Window*,
		overdrive::input::Mouse*
	> mMouseRegistry;

	void onMouseButton(
		GLFWwindow* handle, 
		int button, 
		int action, 
		int modifiers
	) {
		using namespace overdrive;

		if (auto w = video::Window::getFromHandle(handle)) {
			auto it = mMouseRegistry.find(w);
			assert(it != mMouseRegistry.end());
			it->second->setButtonState((input::Mouse::eButton)button, (action == GLFW_PRESS));

			switch (action) {
			case GLFW_PRESS:
				core::Channel::broadcast(input::Mouse::OnButtonPress{ (input::Mouse::eButton)button, modifiers, w });
				break;

			case GLFW_RELEASE:
				core::Channel::broadcast(input::Mouse::OnButtonRelease{ (input::Mouse::eButton)button, modifiers, w });
				break;
			}
		}
		else
			gLog.warning() << "received mouse press from unregistered window";
	}

	void onMouseMove(
		GLFWwindow* handle,
		double x,
		double y
	) {
		using namespace overdrive;

		static double previousX = 0;
		static double previousY = 0;

		if (auto w = video::Window::getFromHandle(handle)) {
			auto it = mMouseRegistry.find(w);
			assert(it != mMouseRegistry.end());
			it->second->setPosition(x, y, false); // just update the internals

			core::Channel::broadcast(input::Mouse::OnMove{
				x,
				y,
				x - previousX,
				y - previousY,
				w
			});

			previousX = x;
			previousY = y;
		}
		else
			gLog.warning() << "received mouse press from unregistered window";
	}
}

namespace overdrive {
	namespace input {
		Mouse::Mouse(const video::Window* associatedWindow):
			mAssociatedWindow(associatedWindow)
		{
			for (auto& button : mButtonState)
				button = false;

			registerMouse(associatedWindow, this);

			glfwSetMouseButtonCallback(associatedWindow->getHandle(), &onMouseButton);
			glfwSetCursorPosCallback(associatedWindow->getHandle(), &onMouseMove);
		}

		void Mouse::setButtonState(eButton button, bool pressed) {
			mButtonState[button] = pressed;
		}

		void Mouse::setPosition(double x, double y, bool setCursor) {
			mX = x;
			mY = y;

			if (setCursor)
				glfwSetCursorPos(mAssociatedWindow->getHandle(), mX, mY);
		}

		bool Mouse::operator[](Mouse::eButton button) const {
			return mButtonState[button];
		}

		void Mouse::getPosition(double& x, double& y) const {
			x = mX;
			y = mY;
		}

		bool Mouse::isAssociatedWith(const video::Window* window) const {
			return window == mAssociatedWindow;
		}

		void registerMouse(const video::Window* win, Mouse* m) {
			assert(mMouseRegistry.find(win) == mMouseRegistry.end());
			mMouseRegistry[win] = m;
		}

		void unregisterMouse(const video::Window* win) {
			assert(mMouseRegistry.find(win) != mMouseRegistry.end());
			mMouseRegistry.erase(win);
		}
	}
}