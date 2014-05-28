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
		std::unordered_map<GLFWwindow*, Window*> Window::mHandleRegistry;

		Window::Window() {
		}

		void Window::setWidth(int width) {
			mWidth = width;
		}

		void Window::setHeight(int height) {
			mHeight = height;
		}

		int Window::getWidth() const {
			return mWidth;
		}

		int Window::getHeight() const {
			return mHeight;
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
				mHandleRegistry[mHandle] = this;

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
				mHandleRegistry[mHandle] = this;

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

		void Window::swapBuffers() {
			glfwSwapBuffers(mHandle);
		}

		bool Window::shouldClose() const {
			return false;
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
			case eClientAPI::OPENGL_API:
				glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_API);
				break;

			case eClientAPI::OPENGL_ES_API:
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
			case eOpenGLProfile::OPENGL_ANY_PROFILE:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
				break;

			case eOpenGLProfile::OPENGL_COMPATIBILITY_PROFILE:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
				break;

			case eOpenGLProfile::OPENGL_CORE_PROFILE:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				break;

			default:
				gLog.warning() << "Unsupported openGL profile requested: " << mOpenGLProfile;
			}
		}

		Window* Window::getFromHandle(GLFWwindow* handle) {
			auto it = mHandleRegistry.find(handle);

			if (it == mHandleRegistry.end())
				throw std::runtime_error("handle not registered");

			return it->second;
		}
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::video::Window::eClientAPI& api) {
	switch (api) {
	case overdrive::video::Window::eClientAPI::OPENGL_API:
		os << "openGL";
		break;

	case overdrive::video::Window::eClientAPI::OPENGL_ES_API:
		os << "openGLES";
		break;

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

	default:
		os << "unknown";
	}

	return os;
}

std::ostream& operator << (std::ostream& os, const overdrive::video::Window::eOpenGLProfile& profile) {
	switch (profile) {
	case overdrive::video::Window::eOpenGLProfile::OPENGL_ANY_PROFILE:
		os << "any";
		break;

	case overdrive::video::Window::eOpenGLProfile::OPENGL_COMPATIBILITY_PROFILE:
		os << "compatibility";
		break;

	case overdrive::video::Window::eOpenGLProfile::OPENGL_CORE_PROFILE:
		os << "core";
		break;

	default:
		os << "unknown";
	}

	return os;
}