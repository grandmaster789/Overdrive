#include "engine.h"
#include "channel.h"
#include "logger.h"
#include "../app/application.h"
#include "../opengl.h"

namespace {
	std::string intepretGLFWerrorcode(int code) {
		switch (code) {
		case GLFW_NOT_INITIALIZED:		return "GLFW has not been initialized";
		case GLFW_NO_CURRENT_CONTEXT:	return "No context is current for this thread";
		case GLFW_INVALID_ENUM:			return "One of the arguments to the function was an invalid enum value";
		case GLFW_INVALID_VALUE:		return "One of the arguments to the function was an invalid value";
		case GLFW_OUT_OF_MEMORY:		return "A memory allocation failed";
		case GLFW_API_UNAVAILABLE:		return "GLFW could not find support for the requested client API on the system";
		case GLFW_VERSION_UNAVAILABLE:	return "The requested OpenGL or OpenGL ES version is not available";
		case GLFW_PLATFORM_ERROR:		return "A platform - specific error occurred that does not match any of the more specific categories";
		case GLFW_FORMAT_UNAVAILABLE:	return "The requested format is not supported or available";
		default:
			return std::string("Unknown error code: ") + std::to_string(code);
		}
	}

	void glfwErrorCallback(int error, const char* description) {
		gLogError << "GLFW error (" << error << "): " << description;
	}
}

namespace overdrive {
	namespace core {
		Engine::Engine():
			mRunning(false)
		{
			Channel::add<OnStop>(this);
		}

		Engine::~Engine() {
			Channel::remove<OnStop>(this);
		}

		void Engine::setApplication(app::Application* application) {
			mApplication.reset(application);

			if (mApplication) {
				mApplication->mEngine = this;
				gLog << "Application set: " << application->getName();
			}
		}

		void Engine::add(System* s) {
			add(std::unique_ptr<System>(s));
		}

		void Engine::add(std::unique_ptr<System>&& s) {
			auto it = mSystemLookup.find(s->getName());

			if (it == mSystemLookup.end()) {
				s->mEngine = this;

				mSystemLookup[s->getName()] = s.get();
				mSystems.push_back(std::move(s));
			}
			else
				throw std::runtime_error(std::string("Duplicate system added: ") + s->getName());
		}

		void Engine::remove(const std::string& systemName) {
			System* s = nullptr;

			auto it = mSystemLookup.find(systemName);

			if (it != mSystemLookup.end())
				s = it->second;

			if (s)
				remove(s);
			else
				gLogError << "Cannot remove system '" << systemName << "', not currently registered with the engine";
		}

		void Engine::remove(System* s) {
			// first scan the systemlist for the given system
			auto it = std::remove_if(
				mSystems.begin(),
				mSystems.end(),
				[s](const SystemPtr& ptr) {
					return ptr.get() == s;
				}
			);

			if (it != mSystems.end()) {
				// the system was found, perform a clean shutdown and remove it from the active registry/list
				s->shutdown();

				mSystems.erase(it);

				for (auto jt = mSystemLookup.begin(); jt != mSystemLookup.end(); ++jt) {
					if (jt->second == s) {
						mSystemLookup.erase(jt);
						break;
					}
				}
			}
		}

		bool satisfied(System* s, const std::vector<std::string>& initialized) {
			using namespace std;

			for (const auto& dependency : s->getDependencies()) {
				if (
					find(
						initialized.begin(), 
						initialized.end(), 
						dependency
					) == initialized.end()
				)
					return false; // one or more of the dependencies was not initialized yet
			}

			return true;
		}

		void Engine::run() {
			initialize();

			mRunning = true;

			while (mRunning) {
				for (auto& system: mSystems)
					system->update();
			}

			if (mApplication)
				mApplication->shutdown();

			for (auto it = mInitOrder.rbegin(); it != mInitOrder.rend(); ++it)
				(*it)->shutdown();			
		}

		void Engine::stop() {
			mRunning = false;
		}

		void Engine::operator()(const OnStop&) {
			mRunning = false;
		}

		void Engine::initialize() {
			// [TODO] This probably could be cleaned up

			initLibraries();

			{
				// consolidate settings, load configuration and apply them

				for (const auto& system : mSystems)
					mSettings.options().add(system->mSettings);

				mSettings.load("overdrive.cfg");
			}

			size_t initCount = 0;
			size_t currentPass = 0;
			size_t numSystems = mSystems.size();

			std::vector<std::string> initNames;
			std::vector<int> initState(numSystems, 0);

			initNames.reserve(numSystems);

			size_t current = 0;

			while (true) {
				if (
					(!initState[current]) &&
					(satisfied(mSystems[current].get(), initNames))
				) {
					// initialize the system
					auto currentSystem = mSystems[current].get();
					currentSystem->mEngine = this;
					currentSystem->initialize();

					initNames.push_back(currentSystem->getName());
					mInitOrder.push_back(currentSystem);
					initState[current] = 1;
					++currentPass;
				}

				++current;

				if (current == numSystems) {
					current = 0;

					if (currentPass == initCount)
						throw std::runtime_error("Unable to resolve dependencies during startup; cyclic dependency detected");

					initCount = currentPass;

					if (initCount == numSystems)
						break; // all systems were initialized; we're done here
				}
			}

			if (mApplication) {
				mApplication->mEngine = this;
				mApplication->initialize();
			}
			else {
				// [TODO] -- diagnostic mode
			}
		}

		void Engine::initLibraries() {
			// [NOTE] GLEW requires an active openGL context, so it cannot be initialized here
			//        it is initialized in the Video subsystem instead
			glfwSetErrorCallback(&glfwErrorCallback);

			if (!glfwInit())
				throw std::runtime_error("Failed to initialize GLFW");

			gLog << "GLFW v" << glfwGetVersionString();
		}

		void Engine::shutdownLibraries() {
			glfwTerminate();
		}
	}
}