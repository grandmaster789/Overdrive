#ifndef OVERDRIVE_VIDEO_VIDEO_H
#define OVERDRIVE_VIDEO_VIDEO_H

#include "core/system.h"
#include "core/channel.h"
#include "video/monitor.h"
#include "video/window.h"

namespace overdrive {
	namespace video {
		class Video :
			public core::System,
			public core::MessageHandler<Monitor::OnConnected>,
			public core::MessageHandler<Monitor::OnDisconnected>,
			public core::MessageHandler<Window::OnCreate>,
			public core::MessageHandler<Window::OnClose>
		{
		private:
			struct WindowDeleter;
			struct MonitorDeleter;

		public:
			typedef std::unique_ptr<Window, WindowDeleter> WindowPtr;
			typedef std::unique_ptr<Monitor, MonitorDeleter> MonitorPtr;
			typedef std::vector<WindowPtr> WindowList;
			typedef std::vector<MonitorPtr> MonitorList;

			Video();

			virtual bool initialize() override;
			virtual void update() override;
			virtual void shutdown() override;

			void detectMonitors();
			unsigned int getNumMonitors() const;
			const MonitorList& getMonitorList() const;

			Window* createWindow(unsigned int width, unsigned int height); // creates a window object (does not spawn it yet)
			const WindowList& getWindowList() const;

			void operator()(const Monitor::OnConnected& connected);
			void operator()(const Monitor::OnDisconnected& disconnected);
			void operator()(const Window::OnCreate& created);
			void operator()(const Window::OnClose& closed);

		private:
			// Window stuff
			struct MainWindowSettings {
				unsigned int mMainWindowWidth = 800;
				unsigned int mMainWindowHeight = 600;
				bool mMainWindowFullscreen = false;
			} mMainWindowSettings;

			struct WindowDeleter {
				void operator()(Window* w);
			} mWindowDeleter;;

			WindowList mWindowList; // first in the list is the main window

		private:
			// Monitor stuff
			struct MonitorDeleter {
				void operator()(Monitor* m);
			} mMonitorDeleter;

			MonitorList mMonitorList; // first in the list is the primary monitor
		};
	}
}

#endif
