#ifndef OVERDRIVE_MATH_CONVERSIONS_H
#define OVERDRIVE_MATH_CONVERSIONS_H

#include <cmath>

namespace overdrive {
	namespace math {
		static const float PI = 3.141592653589793f;
		static const float TWO_PI = 6.2831853071795862f;

		inline float toRadians(float value) {
			return value * 0.017453292519943295f;
		}

		inline double toRadians(double value) {
			return value * 0.017453292519943295;
		}

		inline float toDegrees(float value) {
			return value * 57.29577951308232f;
		}

		inline double toDegrees(double value) {
			return value * 57.29577951308232;
		}
	}
}

#endif
