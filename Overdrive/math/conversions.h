#ifndef OVERDRIVE_MATH_CONVERSIONS_H
#define OVERDRIVE_MATH_CONVERSIONS_H

#include <cmath>

namespace overdrive {
	namespace math {
		static const float PI = 3.141592653589793f;
		static const float TWO_PI = 6.2831853071795862f;

		inline float toRadians(float value);
		inline double toRadians(double value);

		inline float toDegrees(float value);
		inline double toDegrees(double value);
	}
}

#include "math/conversions.inl"

#endif
