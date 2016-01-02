#pragma once

#include "../opengl.h"
#include "videomode.h"
#include <utility>
#include <string>
#include <vector>

namespace overdrive {
	namespace video {
		/*
		 * GLFW has an opaque monitor object, which is made somewhat less opaque
		 * via this class. The Video subsystem automatically detects monitors and
		 * provides these as Monitor objects.
		 *
		 * [NOTE] is the gamma ramp stuff useful? I'm leaving it for later
		 *
		 * http://www.glfw.org/docs/latest/monitor.html
		 * http://www.glfw.org/docs/latest/group__monitor.html
		 */

		class Monitor {
		public:
			Monitor(GLFWmonitor* handle);

			GLFWmonitor* getHandle() const;

			bool isPrimary() const;
			const std::string& getName() const;

			std::pair<int, int> getPosition() const; // virtual screen coordinates of the upper left corner of the monitor
			std::pair<int, int> getPhysicalSize() const; // in mm's as reported by the OS
			double getDPI() const;
			
			VideoMode getCurrentVideoMode() const;
			const std::vector<VideoMode>& getSupportedVideoModes() const;

			// ----- Signals -----

			struct OnConnected { GLFWmonitor* mMonitor; }; // When this happens, no Monitor object exists yet
			struct OnDisconnected { Monitor* mMonitor; };

		private:
			GLFWmonitor* mHandle;

			int mPositionX;
			int mPositionY;

			int mPhysicalWidth;
			int mPhysicalHeight;

			std::string mName;

			std::vector<VideoMode> mSupportedVideoModes;
		};

		Monitor* fetch(GLFWmonitor* handle); // [NOTE] implementation is in video.cpp
	}
}
