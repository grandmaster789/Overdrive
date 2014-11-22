#ifndef OVERDRIVE_CORE_SETTINGS_INL
#define OVERDRIVE_CORE_SETTINGS_INL

#include "core/settings.h"
#include "core/logger.h"

namespace overdrive {
	namespace core {
		template <typename T>
		T Settings::get(std::string option_identifier) const {
			if (isAvailable(option_identifier))
				return mVariables[option_identifier.c_str()].as<T>;
			
			gLog.error() << "Failed to find variable: " << identifier;

			return T; // assumes the type can be default-constructed (maybe throw an exception instead?)
		}
	}
}

#endif
