#pragma once

#include "enum_bitfield.h"

namespace overdrive {
	namespace util {
		template <typename T>
		std::underlying_type_t<T> toBitfield(T value) {
			return static_cast<std::underlying_type_t<T>>(value);
		}

		template <typename T, typename... Pack>
		std::underlying_type_t<T> toBitfield(T value, Pack... pack) {
			return
				static_cast<std::underlying_type_t<T>>(value) |
				toBitfield(pack...);
		}
	}
}
