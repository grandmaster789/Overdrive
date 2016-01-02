#pragma once

#include "clamp.h"

namespace overdrive {
	template <typename T>
	T clamp(
		const T& value, 
		const T& minimum, 
		const T& maximum
	) {
		if (value < minimum)
			return minimum;
		
		if (value > maximum)
			return maximum;

		return value;
	}
}
