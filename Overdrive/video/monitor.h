#ifndef OVERDRIVE_VIDEO_MONITOR_H
#define OVERDRIVE_VIDEO_MONITOR_H

#include <utility>
#include <string>
#include <vector>

#include "opengl.h"

namespace overdrive {
	namespace video {
		namespace detail {
			struct VideoMode;
		}

		class Monitor {
		public:
			typedef std::pair<int, int> Dimension; // in approximate millimeters (mm)

			Monitor(GLFWmonitor* mon);
			GLFWmonitor* getHandle() const;

			const std::string& getName() const;

			bool isPrimaryMonitor() const;
			const Dimension& getPhysicalSize() const;

			detail::VideoMode getCurrentVideoMode() const;
			const std::vector<detail::VideoMode>& getSupportedVideoModes() const;
			//void setVideoMode(detail::VideoMode mode); // this is only done at window creation time for fullscreen mode

			void setGamma(float value); // the other gamma functions are pretty horrible imho

		private:
			GLFWmonitor* mHandle;

			bool mIsPrimaryMonitor;	
			std::string mName;
			Dimension mPhysicalSize;
			
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
