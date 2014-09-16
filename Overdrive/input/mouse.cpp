#include "input/mouse.h"
#include "core/logger.h"
#include "core/channel.h"

namespace overdrive {
	namespace input {
		Mouse::Mouse(GLFWwindow* handle) :
			mHandle(handle)
		{
			assert(mHandle != nullptr);

			std::uninitialized_fill(
				std::begin(mButtonState),
				std::end(mButtonState),
				false
			);
		}

		void Mouse::hideCursor() {
			setCursorState(eCursorState::HIDDEN);
		}

		void Mouse::disableCursor() {
			setCursorState(eCursorState::DISABLED);
		}

		void Mouse::restoreCursor() {
			setCursorState(eCursorState::NORMAL);
		}

		void Mouse::setCursorState(eCursorState state) {
			int value = GLFW_CURSOR_NORMAL;

			switch (state) {
			case eCursorState::NORMAL:
				value = GLFW_CURSOR_NORMAL;
				break;

			case eCursorState::HIDDEN:
				value = GLFW_CURSOR_HIDDEN;
				break;

			case eCursorState::DISABLED:
				value = GLFW_CURSOR_DISABLED;
				break;
			}

			glfwSetInputMode(mHandle, GLFW_CURSOR, value);
		}

		Mouse::eCursorState Mouse::getCursorState() const {
			int mode = glfwGetInputMode(mHandle, GLFW_CURSOR);

			switch (mode) {
			case GLFW_CURSOR_HIDDEN:
				return eCursorState::HIDDEN;

			case GLFW_CURSOR_DISABLED:
				return eCursorState::DISABLED;

			case GLFW_CURSOR_NORMAL:
			default:
				return eCursorState::NORMAL;
			}
		}

		void Mouse::setPosition(double x, double y) {
			mX = x;
			mY = y;

			glfwSetCursorPos(mHandle, mX, mY);
		}

		void Mouse::getPosition(double& x, double& y) const {
			x = mX;
			y = mY;
		}

		void Mouse::setInsideClientArea(bool isInside) {
			mIsInsideClientArea = isInside;
		}

		bool Mouse::isInsideClientArea() const {
			return mIsInsideClientArea;
		}

		bool Mouse::operator [] (eButton button) const {
			return mButtonState[button];
		}
		
		GLFWwindow* Mouse::getHandle() const {
			return mHandle;
		}
	}
}