#include "video/video.h"
#include "core/logger.h"
#include "core/engine.h"
#include "opengl.h"

#include "video/monitor.h"
#include "video/window.h"

#include <unordered_map>

namespace {
	overdrive::core::Channel chan; // shorthand
}

//**** Monitor stuff ****//
namespace {
	typedef std::unordered_map<GLFWmonitor*, overdrive::video::Monitor*> MonitorRegistry;
	
	MonitorRegistry mMonitorRegistry; // not really a member, yet not really a global either

	void registerMonitor(overdrive::video::Monitor* m) {
		auto handle = m->getHandle();
		assert(mMonitorRegistry.find(handle) == mMonitorRegistry.end());
		mMonitorRegistry[handle] = m;
	}

	void unregisterMonitor(GLFWmonitor* mon) {
		auto it = mMonitorRegistry.find(mon);
		assert(it != mMonitorRegistry.end());
		mMonitorRegistry.erase(it);
	}

	overdrive::video::Monitor* fetch(GLFWmonitor* handle) {
		assert(mMonitorRegistry.find(handle) != mMonitorRegistry.end());
		return mMonitorRegistry[handle];
	}

	void monitorCallback(GLFWmonitor* mon, int evt) {
		/*
			Note that under windows (only observed with windows 8, but probably present in other versions as well)
			the callback is only called when physically connecting/disconnecting a new monitor - turning it off or on is not sufficient
			to trigger the callback. This will probably be modified in future versions, but for now it's like that :P
		*/

		switch (evt) {
		case GLFW_CONNECTED:
			chan.broadcast(overdrive::video::Monitor::OnConnected{ mon });
			break;

		case GLFW_DISCONNECTED:
			assert(mMonitorRegistry.find(mon) != mMonitorRegistry.end());
			chan.broadcast(overdrive::video::Monitor::OnDisconnected{ mMonitorRegistry.find(mon)->second });
			break;

		default:
			gLog << "unsupported monitor event: " << evt;
		}
	}
}

//**** Window stuff ****//
namespace {
	typedef std::unordered_map<GLFWwindow*, overdrive::video::Window*> WindowRegistry;

	WindowRegistry mWindowRegistry;

	void registerWindow(overdrive::video::Window* win) {
		auto handle = win->getHandle();
		assert(mWindowRegistry.find(handle) == mWindowRegistry.end());
		mWindowRegistry[handle] = win;
	}

	void unregisterWindow(GLFWwindow* win) {
		auto it = mWindowRegistry.find(win);
		assert(it != mWindowRegistry.end());
		mWindowRegistry.erase(it);
	}

	overdrive::video::Window* fetch(GLFWwindow* handle) {
		assert(mWindowRegistry.find(handle) != mWindowRegistry.end());
		return mWindowRegistry[handle];
	}

	void windowPositionCallback(GLFWwindow* handle, int x, int y) {
		chan.broadcast(overdrive::video::Window::OnMove{ fetch(handle), x, y });
	}

	void windowSizeCallback(GLFWwindow* handle, int newWidth, int newHeight) {
		auto win = fetch(handle);

		win->setWidth(newWidth);
		win->setHeight(newHeight);

		chan.broadcast(overdrive::video::Window::OnResize{ win, newWidth, newHeight });
	}

	void windowCloseCallback(GLFWwindow* handle) {
		chan.broadcast(overdrive::video::Window::OnClose{ fetch(handle) });
	}

	void windowRefreshCallback(GLFWwindow* handle) {
		chan.broadcast(overdrive::video::Window::OnRefresh{ fetch(handle) });
	}

	void windowFocusCallback(GLFWwindow* handle, int isFocus) {
		auto win = fetch(handle);

		if (isFocus)
			chan.broadcast(overdrive::video::Window::OnFocus{ win });
		else
			chan.broadcast(overdrive::video::Window::OnDeFocus{ win });
	}

	void windowIconifyCallback(GLFWwindow* handle, int isIconify) {
		auto win = fetch(handle);

		if (isIconify)
			chan.broadcast(overdrive::video::Window::OnIconify{ win });
		else
			chan.broadcast(overdrive::video::Window::OnRestore{ win });
	}

	void windowFramebufferSizeCallback(GLFWwindow* handle, int newWidth, int newHeight) {
		chan.broadcast(overdrive::video::Window::OnFramebufferResize{ fetch(handle), newWidth, newHeight });
	}
}

namespace overdrive {
	namespace video {
		Video::Video():
			System("Video")
		{
			registerSetting<unsigned int>("Width", &mMainWindowSettings.mMainWindowWidth);
			registerSetting<unsigned int>("Height", &mMainWindowSettings.mMainWindowHeight);
			registerSetting<bool>("Fullscreen", &mMainWindowSettings.mMainWindowFullscreen);
		}

		bool Video::initialize() {
			System::initialize();
			
			// initialize monitor management
			detectMonitors();
			glfwSetMonitorCallback(&monitorCallback);

			// initialize main window
			auto mainWindow = createWindow(
				mMainWindowSettings.mMainWindowWidth, 
				mMainWindowSettings.mMainWindowHeight
			);

			// default to an openGL 4.3 context
			Window::mCreationHints.mContextVersionMajor = 4;
			Window::mCreationHints.mContextVersionMinor = 3;

			// spawn the window
			if (mMainWindowSettings.mMainWindowFullscreen) {
				auto primaryMonitor = mMonitorList.front().get();
				mainWindow->createFullscreen(primaryMonitor);
			}
			else
				mainWindow->create();

			// show what we actually got
			gLog.debug() 
				<< "Context info: " << mainWindow->getClientAPI()
				<< ", v" << mainWindow->getContextVersionMajor()
				<< "." << mainWindow->getContextVersionMinor()
				<< "r" << mainWindow->getContextRevision()
				<< ", " << mainWindow->getContextRobustness();

			// configure module updates
			mEngine->updateSystem(this, true, false); // repeatedly update this subsystem

			return true;
		}

		void Video::update() {
			// activate contexts and update window contents
			for (auto it = mWindowList.begin(); it != mWindowList.end(); ) {
				auto window = it->get();

				if (window->shouldClose()) {
					it = mWindowList.erase(it);
				}
				else {
					// render
					window->makeCurrent();

					// swap buffers
					window->swapBuffers();

					++it;
				}
			}

			if (mWindowList.empty())
				chan.broadcast(core::Engine::OnStop{});

			glfwPollEvents();
		}

		void Video::shutdown() {
			System::shutdown();
		}

		void Video::detectMonitors() {
			GLFWmonitor* primaryMonitorHandle = glfwGetPrimaryMonitor();
			
			int numMonitors = 0;
			GLFWmonitor** monitors = glfwGetMonitors(&numMonitors);

			gLog.info() << "Detected " << numMonitors << " monitors";

			MonitorPtr primaryMonitor(new Monitor(primaryMonitorHandle), mMonitorDeleter);
			registerMonitor(primaryMonitor.get());
			mMonitorList.emplace_back(std::move(primaryMonitor));

			auto& primaryInfo = mMonitorList.back();

			gLog.info() << "Primary monitor: " << primaryInfo->getName();
			//gLog.info() << "(" << primaryInfo->getWidth() << "mm x" << primaryInfo->getHeight() << "mm)";
			//gLog.info() << "[" << primaryInfo->getXPos() << ", " << primaryInfo->getYPos() << "]";

			for (int i = 0; i < numMonitors; ++i) {
				if (monitors[i] != primaryMonitorHandle) {
					auto currentMonitorHandler = monitors[i];

					MonitorPtr monitor(new Monitor(currentMonitorHandler), mMonitorDeleter);
					registerMonitor(monitor.get());
					mMonitorList.emplace_back(std::move(monitor));
				}
			}
		}

		unsigned int Video::getNumMonitors() const {
			return mMonitorList.size();
		}

		const Video::MonitorList& Video::getMonitorList() const {
			return mMonitorList;
		}

		Window* Video::createWindow(unsigned int width, unsigned int height) {
			WindowPtr win(new Window, mWindowDeleter);
			win->setSize(width, height);
			Window* result = win.get();
			
			mWindowList.emplace_back(std::move(win));
			
			return result;
		}

		const Video::WindowList& Video::getWindowList() const {
			return mWindowList;
		}

		//**** Handlers ****//

		void Video::operator()(const Monitor::OnConnected& connected) {
			MonitorPtr monitor(new Monitor(connected.mMonitor), mMonitorDeleter);
			gLog.debug() << "Monitor connected: " << monitor->getName();
			registerMonitor(monitor.get());
			mMonitorList.emplace_back(std::move(monitor));
		}

		void Video::operator()(const Monitor::OnDisconnected& disconnected) {
			gLog.debug() << "Monitor disconnected: " << disconnected.mMonitor->getName();
			
			for (auto it = mMonitorList.begin(); it != mMonitorList.end(); ++it) {
				if (it->get() == disconnected.mMonitor) {
					mMonitorList.erase(it);
					return;
				}
			}
		}

		void Video::operator()(const Window::OnCreate& created) {
			GLFWwindow* handle = created.mWindow->getHandle();

			registerWindow(created.mWindow);

			//set all callbacks
			glfwSetWindowCloseCallback(handle, &windowCloseCallback);
			glfwSetWindowFocusCallback(handle, &windowFocusCallback);
			glfwSetWindowIconifyCallback(handle, &windowIconifyCallback);
			glfwSetWindowPosCallback(handle, &windowPositionCallback);
			glfwSetWindowRefreshCallback(handle, &windowRefreshCallback);
			glfwSetWindowSizeCallback(handle, &windowSizeCallback);
			glfwSetFramebufferSizeCallback(handle, &windowFramebufferSizeCallback);
		}

		void Video::operator()(const Window::OnClose& closed) {
			gLog.debug() << "Window closed: " << closed.mWindow->getTitle();

			glfwSetWindowShouldClose(closed.mWindow->getHandle(), GL_TRUE);
		}

		void Video::WindowDeleter::operator()(Window* w) {
			for (auto it = mWindowRegistry.begin(); it != mWindowRegistry.end(); ++it) {
				if (it->second == w) {
					mWindowRegistry.erase(it);
					break;
				}
			}

			glfwDestroyWindow(w->getHandle());

			delete w;
		}

		void Video::MonitorDeleter::operator()(Monitor* m) {
			for (auto it = mMonitorRegistry.begin(); it != mMonitorRegistry.end(); ++it) {
				if (it->second == m) {
					mMonitorRegistry.erase(it);
					break;
				}
			}

			delete m;
		}
	}
}