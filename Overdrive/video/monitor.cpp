#include "video/monitor.h"
#include <cassert>

namespace overdrive {
	namespace video {
		Monitor::Monitor(GLFWmonitor* mon):
			mHandle(mon)
		{
			mIsPrimaryMonitor = (mHandle == glfwGetPrimaryMonitor());
			mName = glfwGetMonitorName(mHandle);
			glfwGetMonitorPhysicalSize(mHandle, &mPhysicalSize.first, &mPhysicalSize.second);
			
			int numModes;
			const GLFWvidmode* modes = glfwGetVideoModes(mHandle, &numModes);
			
			for (int i = 0; i < numModes; ++i)
				mSupportedVideoModes.emplace_back(detail::VideoMode(modes[i]));
		}

		GLFWmonitor* Monitor::getHandle() const {
			return mHandle;
		}

		const std::string& Monitor::getName() const {
			return mName;
		}

		bool Monitor::isPrimaryMonitor() const {
			return mIsPrimaryMonitor;
		}

		const Monitor::Dimension& Monitor::getPhysicalSize() const {
			return mPhysicalSize;
		}

		detail::VideoMode Monitor::getCurrentVideoMode() const {
			const GLFWvidmode* mode = glfwGetVideoMode(mHandle);
			return detail::VideoMode(mode);
		}

		const std::vector<detail::VideoMode>& Monitor::getSupportedVideoModes() const {
			return mSupportedVideoModes;
		}

		void Monitor::setGamma(float value) {
			glfwSetGamma(mHandle, value);
		}

		namespace detail {
			VideoMode::VideoMode() {
			}

			VideoMode::VideoMode(const GLFWvidmode source):
				mWidth{ source.width },
				mHeight{ source.height },

				mRedBits{ source.redBits },
				mGreenBits{ source.greenBits },
				mBlueBits{ source.blueBits },

				mRefreshRate{ source.refreshRate }
			{
			}

			VideoMode::VideoMode(const GLFWvidmode* source):
				mWidth{ source->width },
				mHeight{ source->height },

				mRedBits{ source->redBits },
				mGreenBits{ source->greenBits },
				mBlueBits{ source->blueBits },

				mRefreshRate{ source->refreshRate }
			{
			}
		}
	}
}