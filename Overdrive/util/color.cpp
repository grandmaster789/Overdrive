#include "util/color.h"

namespace overdrive {
	namespace util {
		Color::Color(
			std::uint8_t r,
			std::uint8_t g,
			std::uint8_t b,
			std::uint8_t a
		) :
			mRed(r),
			mGreen(g),
			mBlue(b),
			mAlpha(a)
		{
		}
	}
}