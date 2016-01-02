#pragma once

#include "settings.h"
#include "logger.h"

namespace overdrive {
	namespace core {
		template <typename T>
		T Settings::get(const std::string& option_identifier) const {
			if (isAvailable(option_identifier))
				return mVariables[option_identifier.c_str()].as<T>;

			gLogError << "Failed to find variable: " << identifier;

			return T; // assumes the type can be default-constructed (maybe throw an exception instead?)
		}
	}
}
