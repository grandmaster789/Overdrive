#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace overdrive {
	namespace util {
		// SFINAE test for type functionality

		// "Supports" allows for compile-time switching based on type traits (see unit test for examples)
		// In effect, this helps with compile-time code reflection (up to a point)
		//
		// Works best with 'transparant functors', such as proposed for C++14
		// Note that the MSVC STL already implements this for all standard functors (equal_to, less, ...)
		template <typename Fn>
		struct Supports;

		template <typename T> using supports_equality = Supports<std::equal_to<>(T, T)>;
		template <typename T> using supports_less = Supports<std::less<>(T, T)>;
	}
}

#include "supports.inl"