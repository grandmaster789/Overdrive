#pragma once

#include "../core/system.h"
#include "../core/channel.h"
#include "monitor.h"
#include "window.h"

namespace overdrive {
	namespace video {
		class Video:
			public core::System,
			public MessageHandler<Monitor::OnConnected>,
			public MessageHandler<Monitor::OnDisconnected>
		{
		private:
			struct MonitorDeleter;

		public:
			typedef std::unique_ptr<Monitor, MonitorDeleter> MonitorPtr;
			typedef std::unique_ptr<Window> WindowPtr;
			typedef std::vector<MonitorPtr> MonitorList;
			typedef std::vector<WindowPtr> WindowList;

			Video();

			virtual void initialize() override;
			virtual void update() override;
			virtual void shutdown() override;

			// Monitors
			void detectMonitors();
			size_t getNumMonitors() const;
			const MonitorList& getMonitorList() const;
			const Monitor* getPrimaryMonitor() const;

			// Windows
			WindowHints& getWindowHints();
			const WindowHints& getWindowHints() const;

			Window* createWindow(const std::string& title, int width, int height);						// windowed mode on primary monitor
			Window* createWindow(const std::string& title, const Monitor* m);							// borderless fullscreen
			Window* createWindow(const std::string& title, const Monitor* m, int width, int height);	// custom fullscreen
			
			Window* getMainWindow();
			const Window* getMainWindow() const;

			WindowList& getWindowList();
			const WindowList& getWindowList() const;

			// ----- Handlers -----
			void operator()(const Monitor::OnConnected& connected);
			void operator()(const Monitor::OnDisconnected& disconnected);

		private:
			struct MainWindowSettings {
				unsigned int mWidth = 800;
				unsigned int mHeight = 600;
				bool mFullscreen = false;
				bool mBorderless = false;
			} mMainWindowSettings;

			MonitorList mMonitorList;
			struct MonitorDeleter { void operator()(Monitor* m); } mMonitorDeleter;

			WindowHints mWindowHints;
			WindowList mWindowList;
		};
	}
}