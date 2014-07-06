#include "video/window.h"
#include "core/logger.h"
#include "core/channel.h"
#include "util/checkRange.h"

namespace {
	// static callback functions to tie window events to Window objects
	using namespace overdrive::video;
	overdrive::core::Channel chan;

	void onWindowPosFn(GLFWwindow* handle, int x, int y) {
		if (auto w = Window::getFromHandle(handle))
			chan.broadcast(Window::OnMove { w, x, y });
	}

	void onWindowSizeFn(GLFWwindow* handle, int newWidth, int newHeight) {
		if (auto w = Window::getFromHandle(handle)) {
			w->setWidth(newWidth);
			w->setHeight(newHeight);

			chan.broadcast(Window::OnResize{ w, newWidth, newHeight });
		}
	}

	void onWindowCloseFn(GLFWwindow* handle) {
		if (auto w = Window::getFromHandle(handle))
			chan.broadcast(Window::OnClose{ w });
	}

	void onWindowRefreshFn(GLFWwindow* handle) {
		if (auto w = Window::getFromHandle(handle))
			chan.broadcast(Window::OnRefresh{ w });
	}

	void onWindowFocusFn(GLFWwindow* handle, int isFocus) {
		if (auto w = Window::getFromHandle(handle)) {
			if (isFocus)
				chan.broadcast(Window::OnFocus{ w });
			else
				chan.broadcast(Window::OnDeFocus{ w });
		}
	}

	void onWindowIconifyFn(GLFWwindow* handle, int isIconify) {
		if (auto w = Window::getFromHandle(handle)) {
			if (isIconify)
				chan.broadcast(Window::OnIconify{ w });
			else
				chan.broadcast(Window::OnRestore{ w });
		}
	}

	void onWindowFrameBufferSizeFn(GLFWwindow* handle, int newWidth, int newHeight) {
		if (auto w = Window::getFromHandle(handle))
			chan.broadcast(Window::OnResize{ w, newWidth, newHeight });
	}
}

namespace overdrive {
	namespace video {
		Window::CreationHints Window::mCreationHints;
		std::vector<std::pair<GLFWwindow*, Window*>> Window::mHandleRegistry;

		Window::Window() {
		}

		void Window::setWidth(int width) {
			setSize(width, mHeight);
		}

		void Window::setHeight(int height) {
			setSize(mWidth, height);
		}

		int Window::getWidth() const {
			return mWidth;
		}

		int Window::getHeight() const {
			return mHeight;
		}

		bool Window::isFullscreen() const {
			return mIsFullscreen;
		}

		const std::string& Window::getTitle() const {
			return mTitle;
		}

		void Window::create() {
			if (mHandle != nullptr)
				throw std::runtime_error("Window already created");

			mIsFullscreen = false;

			mCreationHints.apply();

			mHandle = glfwCreateWindow(
				mWidth, 
				mHeight, 
				mTitle.c_str(), 
				nullptr,	//not running fullscreen, so no specific monitor associated
				nullptr		//no shared context
			);

			if (mHandle) {
				mHandleRegistry.push_back(std::make_pair(mHandle, this));

				//set all callbacks
				glfwSetWindowCloseCallback(mHandle, onWindowCloseFn);
				glfwSetWindowFocusCallback(mHandle, onWindowFocusFn);
				glfwSetWindowIconifyCallback(mHandle, onWindowIconifyFn);
				glfwSetWindowPosCallback(mHandle, onWindowPosFn);
				glfwSetWindowRefreshCallback(mHandle, onWindowRefreshFn);
				glfwSetWindowSizeCallback(mHandle, onWindowSizeFn);
				glfwSetFramebufferSizeCallback(mHandle, onWindowFrameBufferSizeFn);

				gLog.info() << "Created window " << getWidth() << "x" << getHeight() << " " << getTitle();

				core::Channel::broadcast(OnCreate{ this });
			}
		}
		
		void Window::createFullscreen(Monitor* m) {
			if (mHandle != nullptr)
				throw std::runtime_error("Window already created");

			mIsFullscreen = true;

			mCreationHints.apply();

			mHandle = glfwCreateWindow(
				mWidth,
				mHeight,
				mTitle.c_str(),
				m->getHandle(),
				nullptr		//no shared context
			);

			if (mHandle) {
				mHandleRegistry.push_back(std::make_pair(mHandle, this));

				//set all callbacks
				glfwSetWindowCloseCallback(mHandle, onWindowCloseFn);
				glfwSetWindowFocusCallback(mHandle, onWindowFocusFn);
				glfwSetWindowIconifyCallback(mHandle, onWindowIconifyFn);
				glfwSetWindowPosCallback(mHandle, onWindowPosFn);
				glfwSetWindowRefreshCallback(mHandle, onWindowRefreshFn);
				glfwSetWindowSizeCallback(mHandle, onWindowSizeFn);
				glfwSetFramebufferSizeCallback(mHandle, onWindowFrameBufferSizeFn);

				gLog.info() << "Created window " << getWidth() << "x" << getHeight() << " " << getTitle();
			}
		}

		void Window::destroy() {
			assert(mHandle);

			// first remove this window from the handle registry
			for (auto it = mHandleRegistry.begin(); it != mHandleRegistry.end(); ++it) {
				if (it->second == this) {
					mHandleRegistry.erase(it);
					break;
				}
			}

			// then actually destroy the window
			glfwDestroyWindow(mHandle);
		}

		void Window::swapBuffers() const {
			assert(mHandle);

			glfwSwapBuffers(mHandle);
		}

		void Window::makeCurrent() const {
			assert(mHandle);

			glfwMakeContextCurrent(mHandle);
		}

		bool Window::shouldClose() const {
			if (mHandle)
				return (glfwWindowShouldClose(mHandle) != 0);
			else
				return false;
		}

		bool Window::isFocused() const {
			assert(mHandle);
			return (glfwGetWindowAttrib(mHandle, GLFW_FOCUSED) != 0);
		}

		bool Window::isIconified() const {
			assert(mHandle);
			return (glfwGetWindowAttrib(mHandle, GLFW_ICONIFIED) != 0);
		}

		bool Window::isVisible() const {
			assert(mHandle);
			return (glfwGetWindowAttrib(mHandle, GLFW_VISIBLE) != 0);
		}

		bool Window::isResizable() const {
			assert(mHandle);
			return (glfwGetWindowAttrib(mHandle, GLFW_RESIZABLE) != 0);
		}

		bool Window::isDecorated() const {
			assert(mHandle);
			return (glfwGetWindowAttrib(mHandle, GLFW_DECORATED) != 0);
		}

		void Window::getPosition(int& x, int& y) const {
			assert(mHandle);
			glfwGetWindowPos(mHandle, &x, &y);
		}

		void Window::getSize(int& width, int& height) const {
			assert(mHandle);
			glfwGetWindowSize(mHandle, &width, &height);
		}

		void Window::getFramebufferSize(int& width, int& height) const {
			assert(mHandle);
			glfwGetFramebufferSize(mHandle, &width, &height);
		}

		Window::eClientAPI Window::getClientAPI() const {
			assert(mHandle);

			switch (glfwGetWindowAttrib(mHandle, GLFW_CLIENT_API)) {
			case GLFW_OPENGL_API:
				return eClientAPI::OPENGL;

			case GLFW_OPENGL_ES_API:
				return eClientAPI::OPENGL_ES;

			default:
				return eClientAPI::UNKNOWN;
			}
		}

		int Window::getContextVersionMajor() const {
			assert(mHandle);
			return glfwGetWindowAttrib(mHandle, GLFW_CONTEXT_VERSION_MAJOR);
		}

		int Window::getContextVersionMinor() const {
			assert(mHandle);
			return glfwGetWindowAttrib(mHandle, GLFW_CONTEXT_VERSION_MINOR);
		}

		int Window::getContextRevision() const {
			assert(mHandle);
			return glfwGetWindowAttrib(mHandle, GLFW_CONTEXT_REVISION);
		}

		bool Window::isOpenGLForwardCompatible() const {
			assert(mHandle);
			return (glfwGetWindowAttrib(mHandle, GLFW_OPENGL_FORWARD_COMPAT) == GL_TRUE);
		}

		bool Window::isOpenGLDebugContext() const {
			assert(mHandle);
			return (glfwGetWindowAttrib(mHandle, GLFW_OPENGL_DEBUG_CONTEXT) == GL_TRUE);
		}

		Window::eContextRobustness Window::getContextRobustness() const {
			assert(mHandle);
			switch (glfwGetWindowAttrib(mHandle, GLFW_CONTEXT_ROBUSTNESS)) {
			case GLFW_LOSE_CONTEXT_ON_RESET:
				return eContextRobustness::LOSE_CONTEXT_ON_RESET;

			case GLFW_NO_RESET_NOTIFICATION:
				return eContextRobustness::NO_RESET_NOTIFICATION;

			case GLFW_NO_ROBUSTNESS:
				return eContextRobustness::NO_ROBUSTNESS;

			default:
				return eContextRobustness::UNKNOWN;
			}
		}

		Window::eOpenGLProfile Window::getOpenGLProfile() const {
			assert(mHandle);
			switch (glfwGetWindowAttrib(mHandle, GLFW_OPENGL_PROFILE)) {
			case GLFW_OPENGL_CORE_PROFILE:
				return eOpenGLProfile::CORE;

			case GLFW_OPENGL_COMPAT_PROFILE:
				return eOpenGLProfile::COMPATIBILITY;

			case GLFW_OPENGL_ANY_PROFILE:
				return eOpenGLProfile::ANY;

			default:
				return eOpenGLProfile::UNKNOWN;
			}
		}

		void Window::setTitle(std::string title) {
			if (title == mTitle)
				return;

			mTitle = std::move(title);

			if (mHandle)
				glfwSetWindowTitle(mHandle, mTitle.c_str());
		}

		void Window::setPosition(int x, int y) {
			assert(mHandle);
			glfwSetWindowPos(mHandle, x, y);
		}

		void Window::setSize(int width, int height) {
			mWidth = width;
			mHeight = height;

			if (mHandle)
				glfwSetWindowSize(mHandle, mWidth, mHeight);
		}

		void Window::iconify() {
			assert(mHandle);
			glfwIconifyWindow(mHandle);
		}

		void Window::restore() {
			assert(mHandle);
			glfwRestoreWindow(mHandle);
		}

		void Window::show() {
			assert(mHandle);
			glfwShowWindow(mHandle);
		}

		void Window::hide() {
			assert(mHandle);
			glfwHideWindow(mHandle);
		}

		void Window::setDefaultCreationHints() {
			CreationHints defaults; // explicit default values can be found in window.h
			mCreationHints = defaults;
		}

		void Window::CreationHints::apply() {
			using util::checkRange;

			glfwWindowHint(GLFW_RESIZABLE, mResizable ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_VISIBLE, mVisible ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_DECORATED, mDecorated ? GL_TRUE : GL_FALSE);

			// can't really test for correct values before attempting to create a window, to my knowledge...
			// so let's perform minimal checking on user input (docs says these support 0..INT_MAX)
			if (checkRange(mRedBits, 0, INT_MAX))
				glfwWindowHint(GLFW_RED_BITS, mRedBits);
			if (checkRange(mGreenBits, 0, INT_MAX))
				glfwWindowHint(GLFW_GREEN_BITS, mGreenBits);
			if (checkRange(mBlueBits, 0, INT_MAX))
				glfwWindowHint(GLFW_BLUE_BITS, mBlueBits);
			if (checkRange(mAlphaBits, 0, INT_MAX))
				glfwWindowHint(GLFW_ALPHA_BITS, mAlphaBits);
			if (checkRange(mDepthBits, 0, INT_MAX))
				glfwWindowHint(GLFW_DEPTH_BITS, mDepthBits);
			if (checkRange(mStencilBits, 0, INT_MAX))
				glfwWindowHint(GLFW_STENCIL_BITS, mStencilBits);
			
			if (checkRange(mAccumulationRedBits, 0, INT_MAX))
				glfwWindowHint(GLFW_ACCUM_RED_BITS, mAccumulationRedBits);
			if (checkRange(mAccumulationBlueBits, 0, INT_MAX))
				glfwWindowHint(GLFW_ACCUM_GREEN_BITS, mAccumulationGreenBits);
			if (checkRange(mAccumulationBlueBits, 0, INT_MAX))
				glfwWindowHint(GLFW_ACCUM_BLUE_BITS, mAccumulationBlueBits);
			if (checkRange(mAccumulationAlphaBits, 0, INT_MAX))
				glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, mAccumulationAlphaBits);

			if (checkRange(mSamples, 0, INT_MAX))
				glfwWindowHint(GLFW_SAMPLES, mSamples);
			if (checkRange(mRefreshRate, 0, INT_MAX))
				glfwWindowHint(GLFW_REFRESH_RATE, mRefreshRate);

			glfwWindowHint(GLFW_STEREO, mStereo ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_SRGB_CAPABLE, mSRGB ? GL_TRUE : GL_FALSE);

			switch (mClientAPI) {
			case eClientAPI::OPENGL:
				glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_API);
				break;

			case eClientAPI::OPENGL_ES:
				glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_ES_API);
				break;

			default:
				gLog.warning() << "Unsupported client API requested: " << mClientAPI;
				break;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mContextVersionMajor);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mContextVersionMinor);

			switch (mContextRobustness) {
			case eContextRobustness::NO_ROBUSTNESS:
				glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS);
				break;

			case eContextRobustness::NO_RESET_NOTIFICATION:
				glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_RESET_NOTIFICATION);
				break;

			case eContextRobustness::LOSE_CONTEXT_ON_RESET:
				glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
				break;

			default:
				gLog.warning() << "Unsupported context robustness setting requested: " << mContextRobustness;
			}

			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, mOpenGLForwardCompatible ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, mOpenGLDebugContext ? GL_TRUE : GL_FALSE);
			
			switch (mOpenGLProfile) {
			case eOpenGLProfile::ANY:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
				break;

			case eOpenGLProfile::COMPATIBILITY:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
				break;

			case eOpenGLProfile::CORE:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				break;

			default:
				gLog.warning() << "Unsupported openGL profile requested: " << mOpenGLProfile;
			}
		}

		GLFWwindow* Window::getHandle() const {
			return mHandle;
		}

		Window* Window::getFromHandle(GLFWwindow* handle) {
			for (const auto& pair : mHandleRegistry)
				if (pair.first == handle)
					return pair.second;

			throw std::runtime_error("Handle not found in window registry");

			return nullptr;
		}
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::video::Window::eClientAPI& api) {
	switch (api) {
	case overdrive::video::Window::eClientAPI::OPENGL:
		os << "openGL";
		break;

	case overdrive::video::Window::eClientAPI::OPENGL_ES:
		os << "openGLES";
		break;

	case overdrive::video::Window::eClientAPI::UNKNOWN:
	default:
		os << "unknown";
	}

	return os;
}

std::ostream& operator << (std::ostream& os, const overdrive::video::Window::eContextRobustness& robust) {
	switch (robust) {
	case overdrive::video::Window::eContextRobustness::NO_ROBUSTNESS:
		os << "no robustness";
		break;

	case overdrive::video::Window::eContextRobustness::NO_RESET_NOTIFICATION:
		os << "no reset notification";
		break;

	case overdrive::video::Window::eContextRobustness::LOSE_CONTEXT_ON_RESET:
		os << "lose context on reset";
		break;

	case overdrive::video::Window::eContextRobustness::UNKNOWN:
	default:
		os << "unknown";
	}

	return os;
}

std::ostream& operator << (std::ostream& os, const overdrive::video::Window::eOpenGLProfile& profile) {
	switch (profile) {
	case overdrive::video::Window::eOpenGLProfile::ANY:
		os << "any";
		break;

	case overdrive::video::Window::eOpenGLProfile::COMPATIBILITY:
		os << "compatibility";
		break;

	case overdrive::video::Window::eOpenGLProfile::CORE:
		os << "core";
		break;
		
	case overdrive::video::Window::eOpenGLProfile::UNKNOWN:
	default:
		os << "unknown";
	}

	return os;
}