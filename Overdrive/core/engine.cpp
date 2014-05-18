#include "engine.h"
#include "channel.h"
#include "logger.h"
#include "system.h"

namespace overdrive {
	namespace core {
		Engine::Engine() {
		}

		void Engine::add(System* system) {
			add(std::unique_ptr<System>(system));
		}

		void Engine::add(std::unique_ptr<System>&& system) {
			auto it = mSystemLookup.find(system->getName());
			
			if (it == mSystemLookup.end()) {
				system->mEngine = this;

				mSystemLookup[system->getName()] = system.get();
				mSystems.emplace_back(std::move(system));
			}
			else
				throw std::runtime_error(std::string("Duplicate system added: ") + system->getName());
		}
		
		System* Engine::get(std::string systemName) const {
			auto it = mSystemLookup.find(systemName);

			if (it == mSystemLookup.end())
				return nullptr;
			else
				return it->second;
		}

		void Engine::run() {
			initializeSystems();
			mTaskProcessor.start();
			shutdownSystems();
		}
		
		void Engine::stop() {
			mTaskProcessor.stop();
		}

		void Engine::initializeSystems() {
			for (auto& system : mSystems) {
				gLog << "Initializing: " << system->getName();
				if (!system->initialize())
					gLog.error() << "Failed to initialize";
			}
		}

		void Engine::shutdownSystems() {
			for (auto& system : mSystems) {
				gLog << "Shutting down: " << system->getName();
				system->shutdown();
			}
		}

		void Engine::updateSystem(System* system, bool repeating, bool background) {
			assert(system);

			mTaskProcessor.add(
				make_wrapped(
					[system] { system->update(); }, 
					repeating, 
					background
				)
			);
		}
	}
}