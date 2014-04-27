#ifndef OVERDRIVE_CORE_SYSTEM_H
#define OVERDRIVE_CORE_SYSTEM_H

#include "util/named.h"
#include <boost/program_options.hpp>

namespace overdrive {
	namespace core {
		class Engine;

		// Abstract 'system' (aka module)
		class System:
			public util::Named
		{
		public:
			System(std::string name);
			virtual ~System();

			virtual bool initialize();
			virtual void update();
			virtual void shutdown();

			template <typename T>
			void registerSetting(std::string nameInConfigFile, T* correspondingVariable) {
				std::string configSetting = getName();
				configSetting.append(".");
				configSetting.append(nameInConfigFile);

				mSettings.add_options()
					(configSetting.c_str(), boost::program_options::value<T>(correspondingVariable))
				;
			}

		protected:
			Engine* mEngine;
			boost::program_options::options_description mSettings;
		};
	}
}

#endif
