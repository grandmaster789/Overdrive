#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "system.h"
#include "settings.h"
#include "clock.h"

namespace overdrive {
	namespace app {
		class Application;
	}

	// [TODO] -- Diagnostic mode (or is the unit test sufficient?)

	namespace core {
		class Engine {
		public:
			typedef std::unique_ptr<System> SystemPtr;
			typedef std::vector<SystemPtr> SystemList;
			typedef std::unordered_map<std::string, System*> SystemMapping;

			Engine();
			~Engine();

			void setApplication(app::Application* application);

			void add(System* s); // [NOTE] the Engine will take ownership of the system, no need to delete it later
			void add(std::unique_ptr<System>&& s);
			void remove(const std::string& systemName);
			void remove(System* s);

			template <typename T>
			T* get() const;

			void run();
			void stop();

			const Clock& getClock() const;

			// ----- Signals -----
			struct OnStop {};

			// ----- Handlers -----
			void operator()(const OnStop&);

		private:
			void initialize(); // initializes 3rd party libraries and all subsystems

			// 3rd party libraries (GLFW, ...)
			void initLibraries(); 
			void shutdownLibraries();

			SystemList mSystems;
			SystemMapping mSystemLookup;
			Settings mSettings;
			Clock mClock;
			bool mRunning;

			std::unique_ptr<app::Application> mApplication;

			std::vector<System*> mInitOrder; // order in which the systems were initialized
		};
	}
}
