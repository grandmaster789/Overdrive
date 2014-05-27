#include "core/system.h"
#include "core/logger.h"

namespace overdrive {
	namespace core {
		System::System(std::string name):
			Named{ std::move(name) }
		{
		}

		bool System::initialize() {
			gLog << "Initializing " << getName();
			return true;
		}

		void System::update() {
		}

		void System::shutdown() {
			gLog << "Shutting down " << getName();
		}
	}
}