#pragma once

namespace overdrive {
	template <typename T>
	T clamp(
		const T& value, 
		const T& minimum, 
		const T& maximum
	);
}

#include "clamp.inl"