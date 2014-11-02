#ifndef OVERDRIVE_UTIL_BITMAP_H
#define OVERDRIVE_UTIL_BITMAP_H

#include "opengl.h"

#include <string>
#include <vector>
#include <cstdint>

namespace overdrive {
	namespace util {
		class Bitmap {
		public:
			typedef std::uint8_t Byte;

			enum class eFormat {
				RGB,
				RGBA,
				SINGLE,			// single-channel image (intensity)
				SINGLE_ALPHA	// one channel for intensity and one for alpha
			};

			Bitmap(
				unsigned int width, 
				unsigned int height, 
				eFormat format = eFormat::RGBA, 
				const Byte* rawData = nullptr
			);
			Bitmap(const Bitmap& bmp);
			~Bitmap();

			Bitmap& operator = (const Bitmap&);

			static Bitmap loadFile(const std::string& path);

			unsigned int getWidth() const;
			unsigned int getHeight() const;
			eFormat getFormat() const;

			const Byte* getPixelBuffer() const;

			const Byte* getPixel(	// the number of bytes for a pixel depends on the internal format
				unsigned int x, 
				unsigned int y
			) const; 

			void setPixel(
				unsigned int x, 
				unsigned int y, 
				const Byte* pixelData
			);

		private:
			eFormat mFormat;

			unsigned int mWidth;
			unsigned int mHeight;

			std::vector<Byte> mPixels;
		};
	}
}

#endif