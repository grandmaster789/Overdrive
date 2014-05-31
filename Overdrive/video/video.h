#ifndef OVERDRIVE_VIDEO_VIDEO_H
#define OVERDRIVE_VIDEO_VIDEO_H

#include "core/system.h"
#include "video/window.h"
#include "video/monitor.h"
#include <vector>

namespace overdrive {
	namespace video {
		/*
			The Video subsystem is basically an object-oriented wrapper around GLFW window handling
		 */

		class Video:
			public core::System
		{
		public:
			Video();

			virtual bool initialize() override;
			virtual void update() override;
			virtual void shutdown() override;

			void detectMonitors();

			void addWindow(Window&& w); //adding a window to the video subsystem assigns ownership

		private:
			// Settings read from config file
			int mMainWindowWidth = 800;
			int mMainWindowHeight = 600;
			bool mMainWindowFullscreen = false;

		private:
			std::vector<Monitor> mMonitors; //the first element is the primary monitor; this is filled in detectMonitors()
			std::vector<Window> mWindows;
		};
	}
}

#endif
