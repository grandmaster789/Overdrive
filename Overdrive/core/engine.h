#ifndef OVERDRIVE_CORE_ENGINE_H
#define OVERDRIVE_CORE_ENGINE_H

#include <memory>
#include <vector>
#include <map>
#include "core/channel.h"
#include "core/taskprocessor.h"
#include "core/config.h"

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
			Config mConfig;

			std::unique_ptr<app::Application> mApplication;
		};

		// Note -- if systems are removed regularly, from non-main threads this needs to become threadsafe as well
		//		   I don't expect this to be the typical use case, so I'll leave it for later


		template <typename T>
		T* Engine::get() const {
			T* result = nullptr;

			for (const auto& system : mSystems) {
				result = dynamic_cast<T*>(system.get());
				
				if (result)
					return result;
			}

			return nullptr;
		}
	}
}

#endif
