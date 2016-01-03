#include "stdafx.h"
#include "monitor.h"

namespace overdrive {
	namespace video {
		Monitor::Monitor(GLFWmonitor* handle):
			mHandle(handle)
		{
			glfwGetMonitorPhysicalSize(handle, &mPhysicalWidth, &mPhysicalHeight);
			glfwGetMonitorPos(handle, &mPositionX, &mPositionY);
			
			mName = glfwGetMonitorName(handle);

			int numModes = 0;
			const GLFWvidmode* modes = glfwGetVideoModes(handle, &numModes);
			mSupportedVideoModes.reserve(static_cast<size_t>(numModes));

			for (int i = 0; i < numModes; ++i)
				mSupportedVideoModes.push_back(VideoMode(modes[i]));
		}

		GLFWmonitor* Monitor::getHandle() const {
			return mHandle;
		}

		bool Monitor::isPrimary() const {
			return (glfwGetPrimaryMonitor() == mHandle);
		}

		const std::string& Monitor::getName() const {
			return mName;
		}

		std::pair<int, int> Monitor::getPosition() const {
			return std::make_pair(mPositionX, mPositionY);
		}

		std::pair<int, int> Monitor::getPhysicalSize() const {
			return std::make_pair(mPhysicalWidth, mPhysicalHeight);
		}

		double Monitor::getDPI() const {
			auto currentMode = getCurrentVideoMode();

			return currentMode.mWidth / (mPhysicalWidth / 25.4);
		}

		VideoMode Monitor::getCurrentVideoMode() const {
			return glfwGetVideoMode(mHandle);
		}

		const std::vector<VideoMode>& Monitor::getSupportedVideoModes() const {
			return mSupportedVideoModes;
		}
	}
}