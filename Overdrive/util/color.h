#ifndef OVERDRIVE_UTIL_COLOR_H
#define OVERDRIVE_UTIL_COLOR_H

#include <cstdint>

namespace overdrive {
	namespace util {
		struct Color {
			Color(
				std::uint8_t r,
				std::uint8_t g,
				std::uint8_t b,
				std::uint8_t a = 255
			);

			std::uint8_t mRed;
			std::uint8_t mGreen;
			std::uint8_t mBlue;
			std::uint8_t mAlpha;
		};
	}
}

#endif
