#ifndef OVERDRIVE_MATH_CONVERSIONS_INL
#define OVERDRIVE_MATH_CONVERSIONS_INL

#include "math/conversions.h"

namespace overdrive {
	namespace math {
		float toRadians(float value) {
			return value * 0.017453292519943295f;
		}

		double toRadians(double value) {
			return value * 0.017453292519943295;
		}

		float toDegrees(float value) {
			return value * 57.29577951308232f;
		}

		double toDegrees(double value) {
			return value * 57.29577951308232;
		}
	}
}

#endif
