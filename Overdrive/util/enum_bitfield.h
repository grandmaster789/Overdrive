#pragma once

#include <type_traits>

namespace overdrive {
	namespace util {
		/*
			This converts an enum class to its underlying type and combines all 
			of them in a single bitfield. Doesn't do any real checking, use at
			your own discretion.
		*/
		template <typename T, typename... Pack>
		std::underlying_type_t<T> toBitfield(T value, Pack... pack);
	}
}

#include "enum_bitfield.inl"