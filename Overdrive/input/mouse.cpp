#include "stdafx.h"
#include "mouse.h"
#include "../video/window.h"
#include "../core/logger.h"
#include "../core/channel.h"
#include "../opengl.h"

namespace {
	using overdrive::core::Channel;
	using overdrive::video::Window;
	using overdrive::input::Mouse;

	void mouseButtonCallback(
		GLFWwindow* handle, 
		int button, 
		int action, 
		int mods
	) {
		Window* w = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		Mouse* m = w->getMouse();

		switch (action) {
		case GLFW_PRESS:
			m->mButtonState[button] = true;
			Channel::broadcast(Mouse::OnButtonPress{ m,  button, mods });
			break;

		case GLFW_RELEASE:
			m->mButtonState[button] = false;
			Channel::broadcast(Mouse::OnButtonRelease{ m, button, mods });
			break;

			// [NOTE] are there other events that could occur?
		}
	}

	void mouseScrollCallback(
		GLFWwindow* handle,
		double xoffset,
		double yoffset
	) {
		Window* w = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		Mouse* m = w->getMouse();

		Channel::broadcast(Mouse::OnScroll{ m, xoffset, yoffset });
	}

	void mousePositionCallback(
		GLFWwindow* handle,
		double xpos,
		double ypos
	) {
		Window* w = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		Mouse* m = w->getMouse();

		auto oldPosition = m->getPosition();
		m->mPositionX = xpos;
		m->mPositionY = ypos;

		Channel::broadcast(Mouse::OnMoved{
			m,
			xpos,
			ypos,
			xpos - oldPosition.first,
			ypos - oldPosition.second
		});
	}
}

namespace overdrive {
	namespace input {
		Mouse::Mouse(video::Window* sourceWindow):
			mSourceWindow(sourceWindow),
			mMouseCursor(glfwCreateStandardCursor(GLFW_ARROW_CURSOR), glfwDestroyCursor),
			mPositionX(0),
			mPositionY(0)
		{
			glfwSetMouseButtonCallback(sourceWindow->getHandle(), mouseButtonCallback);
			glfwSetScrollCallback(sourceWindow->getHandle(), mouseScrollCallback);
			glfwSetCursorPosCallback(sourceWindow->getHandle(), mousePositionCallback);
		}

		void Mouse::setCursorState(eCursorState state) {
			int mode = GLFW_CURSOR_NORMAL;

			switch (state) {
			case eCursorState::VISIBLE: mode = GLFW_CURSOR_NORMAL; break;
			case eCursorState::HIDDEN: mode = GLFW_CURSOR_HIDDEN; break;
			case eCursorState::DISABLED: mode = GLFW_CURSOR_DISABLED; break;
			default:
				gLogWarning << "Unsupported cursor state: " << static_cast<std::underlying_type<eCursorState>::type>(state);
			}

			glfwSetInputMode(mSourceWindow->getHandle(), GLFW_CURSOR, mode);
		}

		Mouse::eCursorState Mouse::getCursorState() const {
			int mode = glfwGetInputMode(mSourceWindow->getHandle(), GLFW_CURSOR);

			eCursorState result = eCursorState::VISIBLE;

			switch (mode) {
			case GLFW_CURSOR_NORMAL: result = eCursorState::VISIBLE; break;
			case GLFW_CURSOR_HIDDEN: result = eCursorState::HIDDEN; break;
			case GLFW_CURSOR_DISABLED: result = eCursorState::DISABLED; break;
			default:
				gLogWarning << "Unsupported cursor state: " << mode;
			}

			return result;
		}

		void Mouse::setStandardCursorShape(eCursorShape shape) {
			int value = GLFW_ARROW_CURSOR;

			switch (shape) {
			case eCursorShape::ARROW: value = GLFW_ARROW_CURSOR; break;
			case eCursorShape::IBEAM: value = GLFW_IBEAM_CURSOR; break;
			case eCursorShape::CROSSHAIR: value = GLFW_CROSSHAIR_CURSOR; break;
			case eCursorShape::HAND: value = GLFW_HAND_CURSOR; break;
			case eCursorShape::HRESIZE: value = GLFW_HRESIZE_CURSOR; break;
			case eCursorShape::VRESIZE: value = GLFW_VRESIZE_CURSOR; break;
			default:
				gLogWarning << "Unsupported standard cursor shape: " << static_cast<std::underlying_type<eCursorShape>::type>(shape);
			}

			auto newCursor = glfwCreateStandardCursor(value);
			glfwSetCursor(mSourceWindow->getHandle(), newCursor);
			mMouseCursor.reset(newCursor);
		}

		void Mouse::resetCursorShape() {
			glfwSetCursor(mSourceWindow->getHandle(), nullptr);
			mMouseCursor.reset();
		}

		void Mouse::setPosition(double x, double y) {
			glfwSetCursorPos(mSourceWindow->getHandle(), x, y);
		}

		std::pair<double, double> Mouse::getPosition() const {
			return std::make_pair(mPositionX, mPositionY);
		}

		std::ostream& operator << (std::ostream& os, const Mouse::eCursorState& state) {
			os << "CursorState: ";

			switch (state) {
			case Mouse::eCursorState::VISIBLE: os << "visible"; break;
			case Mouse::eCursorState::HIDDEN: os << "hidden"; break;
			case Mouse::eCursorState::DISABLED: os << "disabled"; break;

			default:
				os << "unknown";
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const Mouse::eCursorShape& shape) {
			os << "CursorShape: ";

			switch (shape) {
			case Mouse::eCursorShape::ARROW: os << "arrow"; break;
			case Mouse::eCursorShape::IBEAM: os << "I-beam"; break;
			case Mouse::eCursorShape::CROSSHAIR: os << "crosshair"; break;
			case Mouse::eCursorShape::HAND: os << "hand"; break;
			case Mouse::eCursorShape::HRESIZE: os << "h resize"; break;
			case Mouse::eCursorShape::VRESIZE: os << "v resize"; break;
			default:
				os << "unknown";
			}

			return os;
		}
	}
}