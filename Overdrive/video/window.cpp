#include "stdafx.h"
#include "window.h"
#include "../core/logger.h"
#include "../core/channel.h"
#include "../input/keyboard.h"
#include "../input/mouse.h"

// ----- Window Callbacks -----
namespace {
	using overdrive::core::Channel;
	using overdrive::video::Window;

	void framebufferSizeCallback(GLFWwindow* handle, int width, int height) {
		Window* w = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		assert(w);
		Channel::broadcast(Window::OnFramebufferResized{ w, width, height });
	}

	void windowCloseCallback(GLFWwindow* handle) {
		Window* w = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		assert(w);
		Channel::broadcast(Window::OnClosed{ w });
	}

	void windowFocusCallback(GLFWwindow* handle, int state) {
		Window* w = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		assert(w);

		switch (state) {
		case GL_TRUE:
			Channel::broadcast(Window::OnFocused{ w });
			break;

		case GL_FALSE:
			Channel::broadcast(Window::OnFocusLost{ w });
			break;

		default:
			gLogWarning << "Unsupported window focus state from callback: " << state;
		}
	}

	void windowIconifyCallback(GLFWwindow* handle, int state) {
		Window* w = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		assert(w);

		switch (state) {
		case GL_TRUE:
			Channel::broadcast(Window::OnIconify{ w });
			break;

		case GL_FALSE:
			Channel::broadcast(Window::OnRestore{ w });
			break;

		default:
			gLogWarning << "Unsupported window iconify state from callback: " << state;
		}
	}

	void windowPositionCallback(GLFWwindow* handle, int x, int y) {
		Window* w = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		assert(w);

		auto oldPosition = w->getPosition();

		Channel::broadcast(Window::OnMoved{ w, oldPosition.first, oldPosition.second, x, y });
	}

	void windowRefreshCallback(GLFWwindow* handle) {
		Window* w = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		assert(w);

		Channel::broadcast(Window::OnRefreshed{ w });
	}

	void windowResizeCallback(GLFWwindow* handle, int width, int height) {
		Window* w = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		assert(w);

		auto oldSize = w->getSize();
		Channel::broadcast(Window::OnResized{ w, oldSize.first, oldSize.second, width, height });
	}
}

namespace overdrive {
	namespace video {
		using core::Channel;

		Window::Window(const std::string& title, int width, int height):
			mTitle(title),
			mWidth(width),
			mHeight(height)
		{
			mHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
			glfwSetWindowUserPointer(mHandle, this);

			glfwGetWindowPos(mHandle, &mPositionX, &mPositionY);

			glfwSetFramebufferSizeCallback(mHandle, &framebufferSizeCallback);
			glfwSetWindowCloseCallback(mHandle, &windowCloseCallback);
			glfwSetWindowFocusCallback(mHandle, &windowFocusCallback);
			glfwSetWindowIconifyCallback(mHandle, &windowIconifyCallback);
			glfwSetWindowPosCallback(mHandle, &windowPositionCallback);
			glfwSetWindowRefreshCallback(mHandle, &windowRefreshCallback);
			glfwSetWindowSizeCallback(mHandle, &windowResizeCallback);

			Channel::add<OnMoved>(this);
			Channel::add<OnResized>(this);

			mKeyboard.reset(new input::Keyboard(this));
			mMouse.reset(new input::Mouse(this));

			makeCurrent();

			Channel::broadcast(OnCreated{ this });
		}

		Window::Window(const std::string& title, const Monitor* m):
			mTitle(title)
		{
			auto videomode = m->getCurrentVideoMode();

			mHandle = glfwCreateWindow(videomode.mWidth, videomode.mHeight, title.c_str(), m->getHandle(), nullptr);
			glfwSetWindowUserPointer(mHandle, this);
			glfwGetWindowPos(mHandle, &mPositionX, &mPositionY);

			mWidth = videomode.mWidth;
			mHeight = videomode.mHeight;

			glfwSetFramebufferSizeCallback(mHandle, &framebufferSizeCallback);
			glfwSetWindowCloseCallback(mHandle, &windowCloseCallback);
			glfwSetWindowFocusCallback(mHandle, &windowFocusCallback);
			glfwSetWindowIconifyCallback(mHandle, &windowIconifyCallback);
			glfwSetWindowPosCallback(mHandle, &windowPositionCallback);
			glfwSetWindowRefreshCallback(mHandle, &windowRefreshCallback);
			glfwSetWindowSizeCallback(mHandle, &windowResizeCallback);

			Channel::add<OnMoved>(this);
			Channel::add<OnResized>(this);

			mKeyboard.reset(new input::Keyboard(this));
			mMouse.reset(new input::Mouse(this));

			makeCurrent();

			Channel::broadcast(OnCreated{ this });
		}

		Window::Window(const std::string& title, const Monitor* m, int width, int height):
			mTitle(title),
			mWidth(width),
			mHeight(height)
		{
			mHandle = glfwCreateWindow(width, height, title.c_str(), m->getHandle(), nullptr);
			glfwSetWindowUserPointer(mHandle, this);
			glfwGetWindowPos(mHandle, &mPositionX, &mPositionY);

			glfwSetFramebufferSizeCallback(mHandle, &framebufferSizeCallback);
			glfwSetWindowCloseCallback(mHandle, &windowCloseCallback);
			glfwSetWindowFocusCallback(mHandle, &windowFocusCallback);
			glfwSetWindowIconifyCallback(mHandle, &windowIconifyCallback);
			glfwSetWindowPosCallback(mHandle, &windowPositionCallback);
			glfwSetWindowRefreshCallback(mHandle, &windowRefreshCallback);
			glfwSetWindowSizeCallback(mHandle, &windowResizeCallback);

			Channel::add<OnMoved>(this);
			Channel::add<OnResized>(this);

			mKeyboard.reset(new input::Keyboard(this));
			mMouse.reset(new input::Mouse(this));

			makeCurrent();

			Channel::broadcast(OnCreated{ this });
		}

		Window::Window(Window&& w):
			mHandle(std::move(w.mHandle)),
			mTitle(std::move(w.mTitle)),
			mWidth(std::move(w.mWidth)),
			mHeight(std::move(w.mHeight)),
			mPositionX(std::move(w.mPositionY)),
			mPositionY(std::move(w.mPositionY))
		{
			w.mHandle = nullptr;
			
			if (mHandle)
				glfwSetWindowUserPointer(mHandle, this);

			Channel::add<OnMoved>(this);
			Channel::add<OnResized>(this);

			mKeyboard.reset(new input::Keyboard(this));
			mMouse.reset(new input::Mouse(this));

			makeCurrent();

			Channel::broadcast(OnCreated{ this });
		}

		Window& Window::operator = (Window&& w) {
			if (mHandle)
				glfwDestroyWindow(mHandle);

			mHandle = std::move(w.mHandle);
			mTitle = std::move(w.mTitle);
			mWidth = std::move(w.mWidth);
			mHeight = std::move(w.mHeight);
			mPositionX = std::move(w.mPositionX);
			mPositionY = std::move(w.mPositionY);

			if (mHandle)
				glfwSetWindowUserPointer(mHandle, this);

			mKeyboard.reset(new input::Keyboard(this));
			mMouse.reset(new input::Mouse(this));

			makeCurrent();

			Channel::broadcast(OnCreated{ this });

			return *this;
		}

		Window::~Window() {
			if (mHandle)
				glfwDestroyWindow(mHandle);

			Channel::remove<OnMoved>(this);
			Channel::remove<OnResized>(this);
		}

		GLFWwindow* Window::getHandle() const {
			return mHandle;
		}

		bool Window::shouldClose() const {
			return (glfwWindowShouldClose(mHandle) != 0);
		}

		void Window::setShouldClose(bool value) {
			glfwSetWindowShouldClose(mHandle, value ? 1 : 0);
		}

		void Window::setTitle(const std::string& title) {
			mTitle = title;
			glfwSetWindowTitle(mHandle, title.c_str());
		}
		
		void Window::setPosition(int x, int y) {
			glfwSetWindowPos(mHandle, x, y);
		}

		void Window::setSize(int width, int height) {
			glfwSetWindowSize(mHandle, width, height);
		}

		void Window::iconify() {
			glfwIconifyWindow(mHandle);
		}

		void Window::restore() {
			glfwRestoreWindow(mHandle);
		}

		void Window::show() {
			glfwShowWindow(mHandle);
		}

		void Window::hide() {
			glfwHideWindow(mHandle);
		}

		namespace {
			static GLFWwindow* gLastActiveContext = nullptr;
		}

		void Window::makeCurrent() {
			// [NOTE] does this need to be threadsafe?
			if (gLastActiveContext != mHandle) {
				glfwMakeContextCurrent(mHandle);
				gLastActiveContext = mHandle;
			}
		}

		void Window::swapBuffers() {
			glfwSwapBuffers(mHandle);
		}

		std::pair<int, int> Window::getPosition() const {
			return std::make_pair(mPositionX, mPositionY);
		}

		std::pair<int, int> Window::getSize() const {
			return std::make_pair(mWidth, mHeight);
		}

		std::pair<int, int> Window::getFramebufferSize() const {
			int width;
			int height;

			glfwGetFramebufferSize(mHandle, &width, &height);

			return std::make_pair(width, height);
		}

		Window::Frame Window::getFrame() const {
			Frame result;

			glfwGetWindowFrameSize(
				mHandle, 
				&result.mLeft, 
				&result.mTop,
				&result.mRight,
				&result.mBottom
			);

			return result;
		}

		Monitor* Window::getMonitor() const {
			return fetch(glfwGetWindowMonitor(mHandle));
		}

		input::Keyboard* Window::getKeyboard() const {
			return mKeyboard.get();
		}

		input::Mouse* Window::getMouse() const {
			return mMouse.get();
		}

		bool Window::isFocused() const {
			return (glfwGetWindowAttrib(mHandle, GLFW_FOCUSED) != 0);
		}

		bool Window::isIconified() const {
			return (glfwGetWindowAttrib(mHandle, GLFW_ICONIFIED) != 0);
		}

		bool Window::isVisible() const {
			return (glfwGetWindowAttrib(mHandle, GLFW_VISIBLE) != 0);
		}

		bool Window::isResizable() const {
			return (glfwGetWindowAttrib(mHandle, GLFW_RESIZABLE) != 0);
		}

		bool Window::isDecorated() const {
			return (glfwGetWindowAttrib(mHandle, GLFW_DECORATED) != 0);
		}

		bool Window::isFloating() const {
			return (glfwGetWindowAttrib(mHandle, GLFW_FLOATING) != 0);
		}

		Window::ContextAttributes Window::getContextAttributes() const {
			ContextAttributes result = {}; // zero-initialize the entire thing

			int api = glfwGetWindowAttrib(mHandle, GLFW_CLIENT_API);
			int versionMajor = glfwGetWindowAttrib(mHandle, GLFW_CONTEXT_VERSION_MAJOR);
			int versionMinor = glfwGetWindowAttrib(mHandle, GLFW_CONTEXT_VERSION_MINOR);
			int versionRevision = glfwGetWindowAttrib(mHandle, GLFW_CONTEXT_REVISION);
			int forwardCompatible = glfwGetWindowAttrib(mHandle, GLFW_OPENGL_FORWARD_COMPAT);
			int debugContext = glfwGetWindowAttrib(mHandle, GLFW_OPENGL_DEBUG_CONTEXT);
			int profile = glfwGetWindowAttrib(mHandle, GLFW_OPENGL_PROFILE);
			int robustness = glfwGetWindowAttrib(mHandle, GLFW_CONTEXT_ROBUSTNESS);

			switch (api) {
			case GLFW_OPENGL_API: result.mAPI = eClientAPI::OPENGL; break;
			case GLFW_OPENGL_ES_API: result.mAPI = eClientAPI::OPENGL_ES; break;
			default:
				gLogError << "Unsupported client API: " << api;
			}

			result.mVersionMajor = versionMajor;
			result.mVersionMinor = versionMinor;
			result.mVersionRevision = versionRevision;

			result.mOpenGLForwardCompatible = (forwardCompatible != GL_FALSE);
			result.mOpenGLDebugContext = (debugContext != GL_FALSE);

			switch (profile) {
			case GLFW_OPENGL_CORE_PROFILE: result.mProfile = eOpenGLProfile::CORE; break;
			case GLFW_OPENGL_COMPAT_PROFILE: result.mProfile = eOpenGLProfile::COMPATIBILITY; break;
			case GLFW_OPENGL_ANY_PROFILE: result.mProfile = eOpenGLProfile::ANY; break; //  if the OpenGL profile is unknown or the context is an OpenGL ES context
			default:
				gLogError << "Unsupported openGL profile: " << profile;
			}

			switch (robustness) {
			case GLFW_LOSE_CONTEXT_ON_RESET: result.mContextRobustness = eContextRobustness::LOSE_CONTEXT_ON_RESET; break;
			case GLFW_NO_RESET_NOTIFICATION: result.mContextRobustness = eContextRobustness::NO_RESET_NOTIFICATION; break;
			case GLFW_NO_ROBUSTNESS: result.mContextRobustness = eContextRobustness::NO_ROBUSTNESS; break;
			default:
				gLogError << "Unsupported context robustness: " << robustness;
			}

			return result;
		}

		Window::FramebufferAttributes Window::getFramebufferAttributes() const {
			FramebufferAttributes result = {}; // zero-initialize the entire thing

			glGetIntegerv(GL_RED_BITS, &result.mRedBits);
			glGetIntegerv(GL_GREEN_BITS, &result.mGreenBits);
			glGetIntegerv(GL_BLUE_BITS, &result.mBlueBits);
			glGetIntegerv(GL_ALPHA_BITS, &result.mAlphaBits);
			glGetIntegerv(GL_DEPTH_BITS, &result.mDepthBits);
			glGetIntegerv(GL_STENCIL_BITS, &result.mStencilBits);
			glGetIntegerv(GL_SAMPLES, &result.mSamples);

			return result;
		}

		void Window::operator()(const OnMoved& moved) {
			if (moved.mWindow == this) {
				// update cached values
				mPositionX = moved.mPositionX;
				mPositionY = moved.mPositionY;
			}
		}

		void Window::operator()(const OnResized& resized) {
			if (resized.mWindow == this) {
				// update cached values
				mWidth = resized.mWidth;
				mHeight = resized.mHeight;
			}
		}
	}
}