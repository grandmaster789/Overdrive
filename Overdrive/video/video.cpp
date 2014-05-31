#include "video/video.h"
#include "core/engine.h"
#include "core/logger.h"
#include "opengl.h"

namespace overdrive {
	namespace video {
		Video::Video() :
			System("Video")
		{
			registerSetting("width", &mMainWindowWidth);
			registerSetting("height", &mMainWindowHeight);
			registerSetting("fullscreen", &mMainWindowFullscreen);
		}

		bool Video::initialize() {
			System::initialize();
			
			if (glfwInit() == GL_FALSE) {
				gLog.error() << "Failed to initialize GLFW";
				return false;
			}

			int glfwMajor;
			int glfwMinor;
			int glfwRevision;

			glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);
			gLog.info() << "GLFW v" << glfwMajor << "." << glfwMinor << "r" << glfwRevision << " initialized";

			detectMonitors();

			Window mainWindow;

			mainWindow.setWidth(mMainWindowWidth);
			mainWindow.setHeight(mMainWindowHeight);
			mainWindow.setTitle("Overdrive");

			//go for openGL 4.0 core by default
			mainWindow.mCreationHints.mContextVersionMajor = 4;
			mainWindow.mCreationHints.mContextVersionMinor = 0;
			mainWindow.mCreationHints.mOpenGLProfile = Window::eOpenGLProfile::OPENGL_CORE_PROFILE;

			if (mMainWindowFullscreen) {
				if (mMonitors.empty()) {
					gLog.warning() << "Cannot go fullscreen - no monitors were found";
					mMainWindowFullscreen = false;
				}
				else
					mainWindow.createFullscreen(&mMonitors[0]);
			}
			
			if (!mMainWindowFullscreen)
				mainWindow.create();

			addWindow(std::move(mainWindow));

			mEngine->updateSystem(this, true, false);

			return true;
		}

		void Video::update() {
			for (const auto& w : mWindows)
				w.swapBuffers();
		}

		void Video::shutdown() {
			System::shutdown();

			glfwTerminate();
		}

		void Video::detectMonitors() {
			int numMonitors;
			GLFWmonitor** detected = glfwGetMonitors(&numMonitors); 
			GLFWmonitor* primary = glfwGetPrimaryMonitor();

			gLog.info() << "Detected " << numMonitors << " monitors";

			mMonitors.emplace_back(Monitor(primary));

			for (int i = 0; i < numMonitors; ++i)
				if (detected[i] != primary)
					mMonitors.emplace_back(Monitor(detected[i]));
		}

		void Video::addWindow(Window&& w) {
			mWindows.emplace_back(std::move(w));
		}
	}
}