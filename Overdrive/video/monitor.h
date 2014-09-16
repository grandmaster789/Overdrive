#ifndef OVERDRIVE_VIDEO_MONITOR_H
#define OVERDRIVE_VIDEO_MONITOR_H

#include <string>
#include <unordered_map>

#include "opengl.h"

namespace overdrive {
	namespace video {
		namespace detail {
			struct VideoMode;
		}

		class Monitor {
		public:
			Monitor(GLFWmonitor* src);

			struct OnConnected {
				GLFWmonitor* mMonitor; // if it just connected, there is no corresponding monitor object yet, so just provide the handle
			};

			struct OnDisconnected {
				Monitor* mMonitor; // disconnecting monitors should have a corresponding monitor object, which should be more convenient to deal with
			};

			int getWidth() const;
			int getHeight() const;

			int getXPos() const;
			int getYPos() const;

			std::string getName() const;

			bool isPrimary() const;

			void setGamma(float value);

			GLFWmonitor* getHandle() const;

			detail::VideoMode getCurrentVideoMode() const;
			const std::vector<detail::VideoMode>& getSupportedVideoModes() const;
			// setting video mode can only be done via fullscreen window creation

		private:
			int mPhysicalWidth = 10;	// in millimeters
			int mPhysicalHeight = 10; // in millimeters

			// position in screen coordinates of the upper left corner of this monitor
			int mXPos = 0;
			int mYPos = 0;

			// Note - I'm ignoring the monitor gamma ramp stuff, I don't consider it useful

			bool mIsPrimaryMonitor = false;

			std::string mName = "Unknown monitor";

			GLFWmonitor* mGLFWhandle = nullptr;

			std::vector<detail::VideoMode> mSupportedVideoModes;
		};

		namespace detail {
			struct VideoMode {
				VideoMode();
				VideoMode(const GLFWvidmode source);
				VideoMode(const GLFWvidmode* source);

				int mWidth = 0;
				int mHeight = 0;

				int mRedBits = 8;
				int mGreenBits = 8;
				int mBlueBits = 8;

				int mRefreshRate = 0;
			};
		}
	}
}

#endif
