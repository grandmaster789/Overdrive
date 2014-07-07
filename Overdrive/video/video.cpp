#include "video/video.h"
#include "core/engine.h"
#include "core/logger.h"
#include "opengl.h"

namespace overdrive {
	namespace video {
		Video::Video() :
			System("Video")
		{
			registerSetting("Width", &mMainWindowWidth);
			registerSetting("Height", &mMainWindowHeight);
			registerSetting("Fullscreen", &mMainWindowFullscreen);
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
			gLog << "GLFW v" << glfwMajor << "." << glfwMinor << "r" << glfwRevision << " initialized";

			detectMonitors();
			
			Window mainWindow;

			mainWindow.setWidth(mMainWindowWidth);
			mainWindow.setHeight(mMainWindowHeight);
			mainWindow.setTitle("Overdrive");

			//go for openGL 4.0 core by default
			mainWindow.mCreationHints.mContextVersionMajor = 4;
			mainWindow.mCreationHints.mContextVersionMinor = 0;
			mainWindow.mCreationHints.mOpenGLProfile = Window::eOpenGLProfile::CORE;

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

			mainWindow.makeCurrent();

			addWindow(std::move(mainWindow));

			// initialize GLEW (must be done after activating an openGL context)
			glewExperimental = GL_TRUE; // this must be set to true for latest openGL extensions
			
			GLenum err = glewInit();
			if (err != GLEW_OK) {
				gLog.error() << "Failed to initialize GLEW: " << glewGetErrorString(err);
				return false;
			}

			gLog.info() << "GLEW v" << glewGetString(GLEW_VERSION);

			// notify engine that this subsystem should be continuously updated on the main thread
			mEngine->updateSystem(this, true, false);

			return true;
		}

		void Video::update() {
			for (const auto& w : mWindows)
				w.swapBuffers();

			glfwPollEvents();
		}

		void Video::shutdown() {
			System::shutdown();

			glfwTerminate();
		}

		void Video::detectMonitors() {
			int numMonitors;
			GLFWmonitor** detected = glfwGetMonitors(&numMonitors); 
			GLFWmonitor* primary = glfwGetPrimaryMonitor();

			gLog << "Detected " << numMonitors << " monitors";

			mMonitors.emplace_back(Monitor(primary));

			for (int i = 0; i < numMonitors; ++i)
				if (detected[i] != primary)
					mMonitors.emplace_back(Monitor(detected[i]));
		}

		void Video::addWindow(Window&& w) {
			mWindows.emplace_back(std::move(w));
		}

		const std::vector<Window>& Video::getWindows() const {
			return mWindows;
		}
	}
}