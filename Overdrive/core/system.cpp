#include "stdafx.h"
#include "system.h"
#include "logger.h"

namespace overdrive {
	namespace core {
		System::System(const std::string& name):
			mName(name)
		{
		}

		void System::initialize() {
			gLog << "Initializing " << getName();
		}

		void System::update() {
		}

		void System::shutdown() {
			gLog << "Shutting down " << getName();
		}

		const std::string& System::getName() const {
			return mName;
		}

		void System::addDependency(const std::string& name) {
			mDependencies.push_back(name);
			// [TODO] -- check for duplicates
		}

		const std::vector<std::string>& System::getDependencies() const {
			return mDependencies;
		}
	}
}