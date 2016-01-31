#pragma once

#include "texture.h"

namespace overdrive {
	namespace render {
		/*
			Remaining texture parameters:
			~ TEXTURE_BASE_LEVEL (partially implemented)
			~ TEXTURE_MAX_LEVEL (barely implemented)
			DEPTH_STENCIL_TEXTURE_MODE
			TEXTURE_MIN_FILTER
			TEXTURE_MAG_FILTER
			TEXTURE_WRAP_S
			TEXTURE_WRAP_T
			TEXTURE_BORDER_COLOR
			TEXTURE_COMPARE_FUNC
			TEXTURE_COMPARE_MODE
			TEXTURE_LOD_BIAS
			TEXTURE_MIN_LOD
			TEXTURE_MAX_LOD
			TEXTURE_SWIZZLE_R
			TEXTURE_SWIZZLE_G
			TEXTURE_SWIZZLE_B
			TEXTURE_SWIZZLE_A
		*/
		class Texture2D {
		public:
			Texture2D(); // providing an empty default constructor allows us to have Texture2D as a class member with no fuss
			Texture2D(eTextureFormat fmt, int width, int height);
			Texture2D(eTextureFormat fmt, int width, int height, const unsigned char* rawData); // [NOTE] this assumes that the raw data is actually what you say it is! Doesn't support compressed formats in particular!
			//Texture2D(eTextureFormat fmt, int width, int height, unsigned char* rawData, size_t numBytes); // [NOTE] this does support compressed formats, but still does only minimal checking
			Texture2D(const gli::texture& tex);
			~Texture2D();

			Texture2D(const Texture2D&) = delete;
			Texture2D(Texture2D&& t);
			Texture2D& operator = (const Texture2D&) = delete;
			Texture2D& operator = (Texture2D&& t);

			GLuint getHandle() const;
			eTextureFormat getFormat() const;
			eTextureFormatType getFormatType() const;
			int getWidth() const;
			int getHeight() const;

			void bind();
			void unbind();

			void bind(int textureUnit);
			void unbind(int textureUnit);

		private:
			GLuint mHandle;
			eTextureFormat mFormat;
			int mWidth;
			int mHeight;
		};

		Texture2D loadTexture2D(const std::string& filepath); // should probably become a full path
	}
}
