#pragma once

#include "../core/system.h"
#include "../core/channel.h"
#include "monitor.h"
#include "window.h"

namespace overdrive {
	class Video:
		public core::System,
		public MessageHandler<video::Monitor::OnConnected>,
		public MessageHandler<video::Monitor::OnDisconnected>
	{
	private:
		struct MonitorDeleter;

	public:
		typedef std::unique_ptr<video::Monitor, MonitorDeleter> MonitorPtr;
		typedef std::unique_ptr<video::Window> WindowPtr;
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
		const video::Monitor* getPrimaryMonitor() const;

		// Windows
		video::WindowHints& getWindowHints();
		const video::WindowHints& getWindowHints() const;

		video::Window* createWindow(const std::string& title, int width, int height);						// windowed mode on primary monitor
		video::Window* createWindow(const std::string& title, const video::Monitor* m);							// borderless fullscreen
		video::Window* createWindow(const std::string& title, const video::Monitor* m, int width, int height);	// custom fullscreen
		
		video::Window* getMainWindow();
		const video::Window* getMainWindow() const;

		WindowList& getWindowList();
		const WindowList& getWindowList() const;

		// ----- Handlers -----
		void operator()(const video::Monitor::OnConnected& connected);
		void operator()(const video::Monitor::OnDisconnected& disconnected);

	private:
		struct MainWindowSettings {
			unsigned int mWidth = 800;
			unsigned int mHeight = 600;
			bool mFullscreen = false;
			bool mBorderless = false;
		} mMainWindowSettings;

		MonitorList mMonitorList;
		struct MonitorDeleter { void operator()(video::Monitor* m); } mMonitorDeleter;

		video::WindowHints mWindowHints;
		WindowList mWindowList;
	};
}