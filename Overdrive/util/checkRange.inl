#ifndef OVERDRIVE_UTIL_CHECKRANGE_INL
#define OVERDRIVE_UTIL_CHECKRANGE_INL

#include "util/checkRange.h"
#include "core/logger.h"

namespace overdrive {
	namespace util {
		template <typename T>
		bool checkRange(
			const T& value,
			const T& minimum,
			const T& maximum,
			typename std::enable_if<std::is_arithmetic<T>::value>::type*
		) {
			if (value < minimum) {
				gLog << "Value outside of range<" << minimum << ".." << maximum << ">: " << value;
				return false;
			}

			if (value > maximum) {
				gLog << "Value outside of range<" << minimum << ".." << maximum << ">: " << value;
				return false;
			}

			return true;
		}

		template <typename T>
		T clamp(
			T value,
			T minimum,
			T maximum,
			typename std::enable_if<std::is_arithmetic<T>::value>::type*
		) {
			if (value < minimum)
				return minimum;

			if (value > maximum)
				return maximum;

			return value;
		}
	}
}

#endif

