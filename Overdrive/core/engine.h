#ifndef OVERDRIVE_CORE_ENGINE_H
#define OVERDRIVE_CORE_ENGINE_H

#include <memory>
#include <vector>
#include <map>
#include "channel.h"
#include "taskprocessor.h"

namespace overdrive {
	namespace core {
		class System;

		class Engine {
		public:
			typedef std::unique_ptr<System> SystemPtr;
			typedef std::vector<SystemPtr> SystemList;
			typedef std::map<std::string, System*> SystemMapping;

			Engine();

			void add(System* system); //note that the Engine will take ownership of the system added
			void add(std::unique_ptr<System>&& system);
			System* get(std::string systemName) const;

			void run();
			void stop();

			void updateSystem(System* system, bool repeating = false, bool background = false);

		private:
			void initializeSystems();
			void shutdownSystems();

			SystemList mSystems;
			SystemMapping mSystemLookup;
			TaskProcessor mTaskProcessor;
		};
	}
}

#endif
