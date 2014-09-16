#include <unordered_map>

#include "input.h"
#include "input/keyboard.h"
#include "input/mouse.h"

#include "video/video.h"

#include "core/channel.h"
#include "core/logger.h"
#include "core/engine.h"

#include "opengl.h"

namespace {
	overdrive::core::Channel chan; // shorthand
}

//***** Keyboard *****//
namespace { 
	typedef std::unordered_map<GLFWwindow*, overdrive::input::Keyboard*> KeyboardRegistry;
	KeyboardRegistry mKeyboardRegistry;

	void registerKeyboard(overdrive::input::Keyboard* kb) {
		auto handle = kb->getHandle();
		assert(mKeyboardRegistry.find(handle) == mKeyboardRegistry.end());
		mKeyboardRegistry[handle] = kb;
	}

	void unregisterKeyboard(GLFWwindow* kb) {
		auto it = mKeyboardRegistry.find(kb);
		assert(it != mKeyboardRegistry.end());
		mKeyboardRegistry.erase(it);
	}

	overdrive::input::Keyboard* fetchKeyboard(GLFWwindow* handle) {
		assert(mKeyboardRegistry.find(handle) != mKeyboardRegistry.end());
		return mKeyboardRegistry[handle];
	}

	// keyboard callbacks
	void keyboardCallback(
		GLFWwindow* handle, 
		int key, 
		int , //scancode
		int action, 
		int modifiers
	) {
		auto kb = fetchKeyboard(handle);

		switch (action) {
		case GLFW_PRESS:
			kb->mKeyState[key] = true;
			chan.broadcast(overdrive::input::Keyboard::OnKeyPress{ key, modifiers, kb });
			break;

		case GLFW_RELEASE:
			kb->mKeyState[key] = false;
			chan.broadcast(overdrive::input::Keyboard::OnKeyRelease{ key, modifiers, kb });
			break;
		}
	}
}

//***** Mouse *****//
namespace {
	typedef std::unordered_map<GLFWwindow*, overdrive::input::Mouse*> MouseRegistry;
	MouseRegistry mMouseRegistry;

	void registerMouse(overdrive::input::Mouse* m) {
		auto handle = m->getHandle();
		assert(mMouseRegistry.find(handle) == mMouseRegistry.end());
		mMouseRegistry[handle] = m;
	}

	void unregisterMouse(GLFWwindow* handle) {
		auto it = mMouseRegistry.find(handle);
		assert(it != mMouseRegistry.end());
		mMouseRegistry.erase(it);
	}

	overdrive::input::Mouse* fetchMouse(GLFWwindow* handle) {
		assert(mMouseRegistry.find(handle) != mMouseRegistry.end());
		return mMouseRegistry[handle];
	}

	using namespace overdrive::input;

	void mouseButtonCallback(
		GLFWwindow* handle,
		int button,
		int action,
		int modifiers
	) {
		auto mouse = fetchMouse(handle);

		mouse->mButtonState[button] = (action == GLFW_PRESS);

		switch (action) {
		case GLFW_PRESS:
			chan.broadcast(Mouse::OnButtonPress{
				(Mouse::eButton)button,
				modifiers,
				mouse 
			});
			break;

		case GLFW_RELEASE:
			chan.broadcast(Mouse::OnButtonRelease{
				(Mouse::eButton)button,
				modifiers,
				mouse
			});
			break;
		}
	}

	void mouseMoveCallback(
		GLFWwindow* handle,
		double x,
		double y
	) {
		static double previousX = 0;
		static double previousY = 0;

		auto mouse = fetchMouse(handle);

		mouse->mX = x;
		mouse->mY = y;

		chan.broadcast(Mouse::OnMove{
			x,
			y,
			x - previousX,
			y - previousY,
			mouse
		});
	}

	void mouseEnterCallback(
		GLFWwindow* handle,
		int enter
	) {
		auto mouse = fetchMouse(handle);

		mouse->setInsideClientArea(enter == GL_TRUE);

		if (enter == GL_TRUE)
			chan.broadcast(Mouse::OnEnter{ mouse });
		else
			chan.broadcast(Mouse::OnLeave{ mouse });
	}

	void mouseScrollCallback(
		GLFWwindow* handle,
		double xScroll,
		double yScroll
	) {
		auto mouse = fetchMouse(handle);

		chan.broadcast(Mouse::OnScroll{
			xScroll,
			yScroll,
			mouse
		});
	}
}

namespace overdrive {
	namespace input {
		Input::Input() :
			System("Input")
		{
		}

		bool Input::initialize() {
			System::initialize();
			
			mEngine->updateSystem(this, true, false); // continually poll joysticks

			return true;
		}

		void Input::update() {
			for (int i = 0; i < GLFW_JOYSTICK_LAST; ++i) {
				if (glfwJoystickPresent(i)) {
					if (mJoystickList[i].getJoystickID() == Joystick::NOT_PRESENT) {
						mJoystickList[i] = Joystick(i);
						chan.broadcast(Joystick::OnConnect{ &mJoystickList[i] });
					}

					mJoystickList[i].poll();
				}
				else {
					if (mJoystickList[i].getJoystickID() != Joystick::NOT_PRESENT) {
						chan.broadcast(Joystick::OnDisconnect{ &mJoystickList[i] });
						mJoystickList[i] = Joystick();
					}
				}
			}
		}

		void Input::shutdown() {

			System::shutdown();
		}

		void Input::operator()(const video::Window::OnCreate& created) {
			auto handle = created.mWindow->getHandle();

			{
				// Register keyboard object and set callbacks
				auto kb = created.mWindow->getKeyboard();

				registerKeyboard(kb);
				glfwSetKeyCallback(handle, &keyboardCallback);
				mKeyboardList.push_back(kb);

				gLog.debug() << "Keyboard object registered";
			}

			{
				// Register mouse object and set callbacks
				auto mouse = created.mWindow->getMouse();

				registerMouse(mouse);
				glfwSetMouseButtonCallback(handle, &mouseButtonCallback);
				glfwSetCursorPosCallback(handle, &mouseMoveCallback);
				glfwSetCursorEnterCallback(handle, &mouseEnterCallback);
				glfwSetScrollCallback(handle, &mouseScrollCallback);
				mMouseList.push_back(mouse);

				gLog.debug() << "Mouse object registered";
			}
		}

		void Input::operator()(const video::Window::OnClose& closed) {
			auto kb = closed.mWindow->getKeyboard();
			auto mouse = closed.mWindow->getMouse();

			mKeyboardList.erase(std::remove(mKeyboardList.begin(), mKeyboardList.end(), kb));
			mMouseList.erase(std::remove(mMouseList.begin(), mMouseList.end(), mouse));

			unregisterKeyboard(closed.mWindow->getHandle());
			unregisterMouse(closed.mWindow->getHandle());
		}
	}
}