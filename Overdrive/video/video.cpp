#include "video.h"
#include "window.h"
#include "../core/channel.h"
#include "../core/logger.h"
#include "../core/engine.h"
#include "../preprocessor.h"
#include <cassert>
#include <unordered_map>

// ----- Monitor Callbacks -----
namespace {
	using overdrive::video::Monitor;

	typedef std::unordered_map<GLFWmonitor*, Monitor*> MonitorMapping;
	MonitorMapping gMonitorMapping; // quasi-global

	void registerMonitor(Monitor* m) {
		auto handle = m->getHandle();
		assert(gMonitorMapping.find(handle) == gMonitorMapping.end());
		gMonitorMapping[handle] = m;
	}

	void unregisterMonitor(GLFWmonitor* handle) {
		auto it = gMonitorMapping.find(handle);
		assert(it != gMonitorMapping.end());
		gMonitorMapping.erase(it);
	}

	void monitorCallback(GLFWmonitor* handle, int evt) {
		using overdrive::core::Channel;
		using overdrive::video::fetch;

		switch (evt) {
		case GLFW_CONNECTED:
			Channel::broadcast(Monitor::OnConnected{ handle });
			break;

		case GLFW_DISCONNECTED:
			Channel::broadcast(Monitor::OnDisconnected{ fetch(handle) });
			break;

		default:
			gLogWarning << "Unknown monitor event received: " << evt;
		}
	}
}

// ----- Video subsystem -----
namespace overdrive {
	namespace video {
		void Video::MonitorDeleter::operator()(Monitor* m) {
			for (auto it = gMonitorMapping.begin(); it != gMonitorMapping.end(); ++it) {
				if (it->second == m) {
					gMonitorMapping.erase(it);
					break;
				}
			}

			delete m; // custom deleter should still delete
		}

		Video::Video():
			System("Video")
		{
			registerSetting("Width", &mMainWindowSettings.mWidth);
			registerSetting("Height", &mMainWindowSettings.mHeight);
			registerSetting("Fullscreen", &mMainWindowSettings.mFullscreen);
			registerSetting("Borderless", &mMainWindowSettings.mBorderless);
		}

		void Video::initialize() {
			System::initialize();

			// initialize monitor management
			detectMonitors();
			glfwSetMonitorCallback(&monitorCallback);

			// initialize window management
			gLog << "Main window settings: " 
				<< mMainWindowSettings.mWidth 
				<< "x" 
				<< mMainWindowSettings.mHeight 
				<< ", " 
				<< std::boolalpha 
				<< mMainWindowSettings.mFullscreen;

			// [NOTE] These settings *could* be configurable, but I'm keeping it straightforward for now
			mWindowHints.mResizable = true;
			mWindowHints.mVisible = true;
			mWindowHints.mDecorated = true;
			mWindowHints.mFocused = true;
			mWindowHints.mAutoIconify = true;
			mWindowHints.mFloating = false;

			mWindowHints.fromCurrentVideoMode(getPrimaryMonitor()); // set bit depths and refresh rate from current video mode

			mWindowHints.mStereo = false;
			mWindowHints.mSamples = 0;
			mWindowHints.mSRGBCapable = true;
			mWindowHints.mDoubleBuffer = true;

			mWindowHints.mClientAPI = eClientAPI::OPENGL;
			mWindowHints.mContextVersionMajor = 4;
			mWindowHints.mContextVersionMinor = 5;

			mWindowHints.mOpenGLForwardCompatible = true;
			mWindowHints.mOpenGLProfile = eOpenGLProfile::CORE;
#ifdef OVERDRIVE_DEBUG
			mWindowHints.mOpenGLDebugContext = true;
#else
			mWindowHints.mOpenGLDebugContext = false;
#endif
			mWindowHints.mContextRobustness = eContextRobustness::NO_ROBUSTNESS;
			mWindowHints.mContextReleaseBehavior = eContextReleaseBehavior::ANY;

			gLogDebug << mWindowHints;

			mWindowHints.apply();

			// create the main window
			if (mMainWindowSettings.mFullscreen) {
				if (mMainWindowSettings.mBorderless)
					createWindow("Overdrive", getPrimaryMonitor());
				else
					createWindow("Overdrive", getPrimaryMonitor(), mMainWindowSettings.mWidth, mMainWindowSettings.mHeight);
			}
			else
				createWindow("Overdrive", mMainWindowSettings.mWidth, mMainWindowSettings.mHeight);

		}

		void Video::update() {
			using core::Channel;

			if (mWindowList.empty())
				Channel::broadcast(core::Engine::OnStop());

			auto it = mWindowList.begin();

			// close everything when the main window closes
			if ((*it)->shouldClose())
				Channel::broadcast(core::Engine::OnStop());

			for (; it != mWindowList.end();) {
				if ((*it)->shouldClose())
					it = mWindowList.erase(it);
				else {
					// The graphics rendering stuff should go in here. For now I'm keeping it the simplest that could possibly work.
					(*it)->makeCurrent();
					// [NOTE] update rendering here
					(*it)->swapBuffers();
					++it;
				}
			}

			glfwPollEvents();
		}

		void Video::shutdown() {
			System::shutdown();
		}

		void Video::detectMonitors() {
			auto primaryHandle = glfwGetPrimaryMonitor();

			int numMonitors = 0;
			auto monitors = glfwGetMonitors(&numMonitors);

			gLog << "Detected " << numMonitors << " monitors";
			mMonitorList.reserve(static_cast<size_t>(numMonitors));

			MonitorPtr primaryMonitor(new Monitor(primaryHandle), mMonitorDeleter);
			registerMonitor(primaryMonitor.get());
			mMonitorList.push_back(std::move(primaryMonitor));

			for (int i = 0; i < numMonitors; ++i) {
				if (monitors[i] != primaryHandle) {
					MonitorPtr monitor(new Monitor(monitors[i]), mMonitorDeleter);
					registerMonitor(monitor.get());
					mMonitorList.push_back(std::move(monitor));
				}
			}
		}

		size_t Video::getNumMonitors() const {
			return mMonitorList.size();
		}

		const Video::MonitorList& Video::getMonitorList() const {
			return mMonitorList;
		}

		const Monitor* Video::getPrimaryMonitor() const {
			return mMonitorList.front().get();
		}

		WindowHints& Video::getWindowHints() {
			return mWindowHints;
		}

		const WindowHints& Video::getWindowHints() const {
			return mWindowHints;
		}

		Window* Video::createWindow(const std::string& title, int width, int height) {
			auto window = std::make_unique<Window>(title, width, height);
			Window* result = window.get();

			mWindowList.push_back(std::move(window));

			return result;
		}

		Window* Video::createWindow(const std::string& title, const Monitor* m) {
			auto window = std::make_unique<Window>(title, m);
			Window* result = window.get();

			mWindowList.push_back(std::move(window));

			return result;
		}

		Window* Video::createWindow(const std::string& title, const Monitor* m, int width, int height) {
			auto window = std::make_unique<Window>(title, m, width, height);
			Window* result = window.get();

			mWindowList.push_back(std::move(window));

			return result;
		}

		Window* Video::getMainWindow() {
			assert(!mWindowList.empty());
			return mWindowList.front().get();
		}

		const Window* Video::getMainWindow() const {
			assert(!mWindowList.empty());
			return mWindowList.front().get();
		}

		Video::WindowList& Video::getWindowList() {
			return mWindowList;
		}

		const Video::WindowList& Video::getWindowList() const {
			return mWindowList;
		}

		void Video::operator()(const Monitor::OnConnected& connected) {
			MonitorPtr ptr(new Monitor(connected.mMonitor), mMonitorDeleter);

			gLogDebug << "Monitor connected: " << ptr->getName();
			registerMonitor(ptr.get());
			mMonitorList.push_back(std::move(ptr));
		}

		void Video::operator()(const Monitor::OnDisconnected& disconnected) {
			gLogDebug << "Monitor disconnected: " << disconnected.mMonitor->getName();

			for (auto it = mMonitorList.begin(); it != mMonitorList.end(); ++it) {
				if (it->get() == disconnected.mMonitor) {
					mMonitorList.erase(it);
					return;
				}
			}
		}

		Monitor* fetch(GLFWmonitor* handle) {
			assert(gMonitorMapping.find(handle) != gMonitorMapping.end());
			return gMonitorMapping[handle];
		}
	}
}