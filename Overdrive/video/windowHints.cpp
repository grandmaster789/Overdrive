#include "windowHints.h"
#include "monitor.h"
#include "../core/logger.h"
#include <string>

namespace overdrive {
	namespace video {
		namespace {
			std::string maybeDontCare(int value) {
				if (value == GLFW_DONT_CARE)
					return "Don't care";
				else
					return std::to_string(value);
			}

			bool checkHintNumber(int value) {
				return (
					(value >= -1) && // <- this equals GLFW_DONT_CARE
					(value <= INT_MAX)
				);
			}
		}

		WindowHints::WindowHints() {
			// The initial values are declared in the header
		}

		void WindowHints::resetToDefaults() {
			(*this) = WindowHints();
			glfwDefaultWindowHints(); // should be equivalent to :apply()
		}
		
		void WindowHints::fromCurrentVideoMode(const Monitor* m) {
			auto mode = m->getCurrentVideoMode();

			mRedBits = mode.mRedBits;
			mGreenBits = mode.mGreenBits;
			mBlueBits = mode.mBlueBits;
			mRefreshRate = mode.mRefreshRate;
		}

		void WindowHints::apply() {
			assert(checkHintNumber(mRedBits));
			assert(checkHintNumber(mGreenBits));
			assert(checkHintNumber(mBlueBits));
			assert(checkHintNumber(mAlphaBits));
			assert(checkHintNumber(mDepthBits));
			assert(checkHintNumber(mStencilBits));
			assert(checkHintNumber(mAccumRedBits));
			assert(checkHintNumber(mAccumGreenBits));
			assert(checkHintNumber(mAccumBlueBits));
			assert(checkHintNumber(mAccumAlphaBits));
			assert(checkHintNumber(mAuxBuffers));
			assert(checkHintNumber(mSamples));
			assert(checkHintNumber(mRefreshRate));

			// [NOTE] I don't believe it's possible to verify the version numbers beforehand. Not entirely sure though

			glfwWindowHint(GLFW_RESIZABLE, mResizable ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_VISIBLE, mVisible ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_DECORATED, mDecorated ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_FOCUSED, mFocused ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_AUTO_ICONIFY, mAutoIconify ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_FLOATING, mFloating ? GL_TRUE : GL_FALSE);

			glfwWindowHint(GLFW_RED_BITS, mRedBits);
			glfwWindowHint(GLFW_GREEN_BITS, mGreenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mBlueBits);
			glfwWindowHint(GLFW_ALPHA_BITS, mAlphaBits);
			glfwWindowHint(GLFW_DEPTH_BITS, mDepthBits);
			glfwWindowHint(GLFW_STENCIL_BITS, mStencilBits);
			glfwWindowHint(GLFW_ACCUM_RED_BITS, mAccumRedBits);
			glfwWindowHint(GLFW_ACCUM_GREEN_BITS, mAccumGreenBits);
			glfwWindowHint(GLFW_ACCUM_BLUE_BITS, mAccumBlueBits);
			glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, mAccumAlphaBits);

			glfwWindowHint(GLFW_AUX_BUFFERS, mAuxBuffers);

			glfwWindowHint(GLFW_SAMPLES, mSamples);
			glfwWindowHint(GLFW_REFRESH_RATE, mRefreshRate);
			glfwWindowHint(GLFW_STEREO, mStereo ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_SRGB_CAPABLE, mSRGBCapable ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_DOUBLEBUFFER, mDoubleBuffer ? GL_TRUE : GL_FALSE);

			int api = GLFW_OPENGL_API;
			int profile = GLFW_OPENGL_ANY_PROFILE;
			int robustness = GLFW_NO_ROBUSTNESS;
			int releaseBehavior = GLFW_ANY_RELEASE_BEHAVIOR;

			switch (mClientAPI) {
			case eClientAPI::OPENGL:
				api = GLFW_OPENGL_API;
				break;

			case eClientAPI::OPENGL_ES:
				api = GLFW_OPENGL_ES_API;
				break;

			default:
				gLogError << "Unsupported openGL API: " << mClientAPI;
			}

			switch (mOpenGLProfile) {
			case eOpenGLProfile::ANY:
				profile = GLFW_OPENGL_ANY_PROFILE;
				break;

			case eOpenGLProfile::COMPATIBILITY:
				profile = GLFW_OPENGL_COMPAT_PROFILE;
				break;

			case eOpenGLProfile::CORE:
				profile = GLFW_OPENGL_CORE_PROFILE;
				break;

			default:
				gLogError << "Unsupported openGL profile: " << mOpenGLProfile;
			}

			switch (mContextRobustness) {
			case eContextRobustness::LOSE_CONTEXT_ON_RESET:
				robustness = GLFW_LOSE_CONTEXT_ON_RESET;
				break;

			case eContextRobustness::NO_RESET_NOTIFICATION:
				robustness = GLFW_NO_RESET_NOTIFICATION;
				break;

			case eContextRobustness::NO_ROBUSTNESS:
				robustness = GLFW_NO_ROBUSTNESS;
				break;

			default:
				gLogError << "Unsupported context robustness: " << mContextRobustness;
			}

			switch (mContextReleaseBehavior) {
			case eContextReleaseBehavior::ANY:
				releaseBehavior = GLFW_ANY_RELEASE_BEHAVIOR;
				break;

			case eContextReleaseBehavior::FLUSH:
				releaseBehavior = GLFW_RELEASE_BEHAVIOR_FLUSH;
				break;

			case eContextReleaseBehavior::NONE:
				releaseBehavior = GLFW_RELEASE_BEHAVIOR_NONE;
				break;

			default:
				gLogError << "Unsupported context release behavior: " << mContextReleaseBehavior;
			}

			glfwWindowHint(GLFW_CLIENT_API, api);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mContextVersionMajor);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mContextVersionMinor);

			glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, robustness);
			glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, releaseBehavior);

			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, mOpenGLForwardCompatible ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, mOpenGLDebugContext ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
		}

		std::ostream& operator << (std::ostream& os, const WindowHints& hints) {
			os << std::boolalpha
				<< "\tWindow hints:"
				<< "\n\tResizable: " << hints.mResizable
				<< "\n\tVisible: " << hints.mVisible
				<< "\n\tDecorated: " << hints.mDecorated
				<< "\n\tFocused: " << hints.mFocused
				<< "\n\tAutoIconify: " << hints.mAutoIconify
				<< "\n\tFloating: " << hints.mFloating

				<< "\n\tBits:"
				<< " " << maybeDontCare(hints.mRedBits)
				<< " " << maybeDontCare(hints.mGreenBits)
				<< " " << maybeDontCare(hints.mBlueBits)
				<< " " << maybeDontCare(hints.mAlphaBits)
				<< " " << maybeDontCare(hints.mDepthBits)
				<< " " << maybeDontCare(hints.mStencilBits)

				// [NOTE] skipping over legacy stuff
				<< "\n\tStereo: " << hints.mStereo
				<< "\n\tSamples: " << maybeDontCare(hints.mSamples)
				<< "\n\tsRGB: " << hints.mSRGBCapable
				<< "\n\tDoubleBuffer: " << hints.mDoubleBuffer
				<< "\n\tRefreshRate: " << maybeDontCare(hints.mRefreshRate)
				<< "\n\tClientAPI: " << hints.mClientAPI
				<< " v" << hints.mContextVersionMajor
				<< "." << hints.mContextVersionMinor
				<< "\n\tForwardCompatible: " << hints.mOpenGLForwardCompatible
				<< "\n\tContextRobustness: " << hints.mContextRobustness
				<< "\n\tContextReleaseBehavior: " << hints.mContextReleaseBehavior;

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eClientAPI& value) {
			switch (value) {
			case eClientAPI::OPENGL:
				os << "openGL";
				break;

			case eClientAPI::OPENGL_ES:
				os << "openGL ES";
				break;

			default:
				os << "unknown - " << static_cast<std::underlying_type<eClientAPI>::type>(value);
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eContextRobustness& value) {
			switch (value) {
			case eContextRobustness::LOSE_CONTEXT_ON_RESET:
				os << "lose context on reset";
				break;

			case eContextRobustness::NO_RESET_NOTIFICATION:
				os << "no reset notification";
				break;

			case eContextRobustness::NO_ROBUSTNESS:
				os << "no robustness";
				break;

			default:
				os << "unknown - " << static_cast<std::underlying_type<eContextRobustness>::type>(value);
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eContextReleaseBehavior& value) {
			switch (value) {
			case eContextReleaseBehavior::ANY:
				os << "any";
				break;

			case eContextReleaseBehavior::FLUSH:
				os << "flush";
				break;

			case eContextReleaseBehavior::NONE:
				os << "none";
				break;

			default:
				os << "unknown - " << static_cast<std::underlying_type<eContextReleaseBehavior>::type>(value);
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eOpenGLProfile& value) {
			switch (value) {
			case eOpenGLProfile::ANY:
				os << "any profile";
				break;

			case eOpenGLProfile::COMPATIBILITY:
				os << "compatibility profile";
				break;

			case eOpenGLProfile::CORE:
				os << "core profile";
				break;

			default:
				os << "unknown - " << static_cast<std::underlying_type<eOpenGLProfile>::type>(value);
			}

			return os;
		}
	}
}

