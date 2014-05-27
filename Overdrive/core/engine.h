#ifndef OVERDRIVE_CORE_ENGINE_H
#define OVERDRIVE_CORE_ENGINE_H

#include <memory>
#include <vector>
#include <map>
#include "core/channel.h"
#include "core/taskprocessor.h"

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
			void remove(std::string systemName);
			void remove(System* system);

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

		// Note -- if systems are removed regularly, from non-main threads this needs to become threadsafe as well
		//		   I don't expect this to be the typical use case, so I'll leave it for later
	}
}

#endif
