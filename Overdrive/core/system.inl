#ifndef OVERDRIVE_CORE_SYSTEM_INL
#define OVERDRIVE_CORE_SYSTEM_INL

#include "core/system.h"

namespace overdrive {
	namespace core {
		template <typename T>
		void System::registerSetting(
			const std::string& nameInConfigFile, 
			T* correspondingVariable
		) {
			std::string configSetting = getName();
			configSetting.append(".");
			configSetting.append(nameInConfigFile);

			mSettings.add_options()
				(configSetting.c_str(), boost::program_options::value<T>(correspondingVariable))
			;
		}
	}
}

#endif
