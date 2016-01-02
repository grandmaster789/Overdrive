#pragma once

#include "channel.h"
#include <boost/program_options.hpp>
#include <vector>

namespace overdrive {
	namespace core {
		class System {
		public:
			friend class Engine;

			typedef boost::program_options::options_description Settings;

			System(const std::string& name);
			System(const System&) = delete;
			System& operator = (const System&) = delete;

			virtual void initialize();
			virtual void update();
			virtual void shutdown();

			const std::string& getName() const;

			template <typename T>
			void registerSetting(const std::string& nameInConfigFile, T* correspondingVar);
			
			// used to determine startup order
			void addDependency(const std::string& name);
			const std::vector<std::string>& getDependencies() const;

		protected:
			Channel mChannel;
			Engine* mEngine; // this is set by the Engine before initializing the system
			Settings mSettings;
			
		private:
			std::string mName;
			std::vector<std::string> mDependencies; 
		};
	}
}

#include "system.inl"