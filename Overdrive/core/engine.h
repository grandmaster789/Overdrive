#ifndef OVERDRIVE_CORE_ENGINE_H
#define OVERDRIVE_CORE_ENGINE_H

#include <memory>
#include <vector>
#include <map>
#include "core/channel.h"
#include "core/taskprocessor.h"
#include "core/settings.h"

namespace overdrive {
	namespace app {
		class Application;
	}

	namespace core {
		class System;

		class Engine {
		public:
			typedef std::unique_ptr<System> SystemPtr;
			typedef std::vector<SystemPtr> SystemList;
			typedef std::map<std::string, System*> SystemMapping;

			Engine();

			void setApplication(app::Application* application); // the Engine will take ownership of the application

			void add(System* system); //note that the Engine will take ownership of the system added
			
			template <typename T>
			T* get() const;

			void remove(std::string systemName);
			void remove(System* system);

			bool initialize(); // initializes all subsystems (except for the Application)
			void run();
			void stop();

			void updateSystem(System* system, bool repeating = false, bool background = false);

			// Signals
			struct OnStop {};

			// Handlers
			void operator()(const OnStop&);

		private:
			void setApplication(std::unique_ptr<app::Application>&& application);
			void add(std::unique_ptr<System>&& system);

			void initializeSystems();
			void shutdownSystems();

			bool initializeDependencies();
			void shutdownDependencies();

			SystemList mSystems;
			SystemMapping mSystemLookup;
			TaskProcessor mTaskProcessor;
			Settings mSettings;

			std::unique_ptr<app::Application> mApplication;
		};
	}
}

#include "engine.inl"

#endif
