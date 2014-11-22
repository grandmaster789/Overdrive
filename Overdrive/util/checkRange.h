#ifndef OVERDRIVE_UTIL_CHECKRANGE_H
#define OVERDRIVE_UTIL_CHECKRANGE_H

#include <cassert>
#include <type_traits>

namespace overdrive {
	namespace util {
		// verify that a value lies in a given range
		template <typename T>
		bool checkRange(
			const T& value,
			const T& minimum,
			const T& maximum,
			typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr	//only provide this function for arithmetic types
		);

		// clamp a value to a given range
		template <typename T>
		T clamp(
			T value,
			T minimum,
			T maximum,
			typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr	//only provide this function for arithmetic types
		);

		// [TODO] repeat? mirror?
	}
}

#include "util/checkRange.inl"

#endif

