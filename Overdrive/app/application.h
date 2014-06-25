#ifndef OVERDRIVE_APP_APPLICATION_H
#define OVERDRIVE_APP_APPLICATION_H

#include "core/system.h"

namespace overdrive {
	namespace app {
		/*
		 * Conceptually an 'application' is very similar to an engine subsystem,
		 * only it can assume that all engine subsystems have been initialized already.
		 * This exists in an attempt to separate 'application logic' from 'library logic'
		 */
		class Application:
			public core::System
		{
		public:
			Application(std::string name);

			// While substems have optional implementations of these, Applications should always provide these
			virtual bool initialize() = 0;
			virtual void update() = 0; 
			virtual void shutdown() = 0;
		};
	}
}

#endif
