#include "video/monitor.h"

namespace overdrive {
	namespace video {
		Monitor::Monitor(GLFWmonitor* src):
			mGLFWhandle(src)
		{
			glfwGetMonitorPhysicalSize(src, &mPhysicalWidth, &mPhysicalHeight);
			glfwGetMonitorPos(src, &mXPos, &mYPos);
			
			mName = glfwGetMonitorName(src);

			mIsPrimaryMonitor = (src == glfwGetPrimaryMonitor());

			int numModes = 0;
			const GLFWvidmode* modes = glfwGetVideoModes(src, &numModes);

			for (int i = 0; i < numModes; ++i)
				mSupportedVideoModes.emplace_back(detail::VideoMode(modes[i]));
		}

		int Monitor::getWidth() const {
			return mPhysicalWidth;
		}

		int Monitor::getHeight() const {
			return mPhysicalHeight;
		}

		int Monitor::getXPos() const {
			return mXPos;
		}

		int Monitor::getYPos() const {
			return mYPos;
		}

		std::string Monitor::getName() const {
			return mName;
		}

		bool Monitor::isPrimary() const {
			return mIsPrimaryMonitor;
		}

		void Monitor::setGamma(float value) {
			glfwSetGamma(mGLFWhandle, value);
		}

		GLFWmonitor* Monitor::getHandle() const {
			return mGLFWhandle;
		}

		detail::VideoMode Monitor::getCurrentVideoMode() const {
			auto mode = glfwGetVideoMode(mGLFWhandle);
			return detail::VideoMode(mode);
		}

		const std::vector<detail::VideoMode>& Monitor::getSupportedVideoModes() const {
			return mSupportedVideoModes;
		}

		namespace detail {
			VideoMode::VideoMode() {
			}

			VideoMode::VideoMode(const GLFWvidmode& source) :
				mWidth(source.width),
				mHeight(source.height),
				mRedBits(source.redBits),
				mGreenBits(source.greenBits),
				mBlueBits(source.blueBits),
				mRefreshRate(source.refreshRate)
			{
			}

			VideoMode::VideoMode(const GLFWvidmode* source) :
				mWidth(source->width),
				mHeight(source->height),
				mRedBits(source->redBits),
				mGreenBits(source->greenBits),
				mBlueBits(source->blueBits),
				mRefreshRate(source->refreshRate)
			{
			}
		}
	}
}