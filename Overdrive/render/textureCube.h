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
		class Texture2D;

		class TextureCube {
		public:
			TextureCube(); // providing an empty default constructor allows us to have Texture2D as a class member with no fuss
			TextureCube(eTextureFormat fmt, int width, int height);
			TextureCube(
				eTextureFormat fmt, 
				int width, 
				int height, 
				const unsigned char* rawPositiveX,
				const unsigned char* rawPositiveY,
				const unsigned char* rawPositiveZ,
				const unsigned char* rawNegativeX,
				const unsigned char* rawNegativeY,
				const unsigned char* rawNegativeZ
			); // [NOTE] use with care, minimal checking
			TextureCube(const gli::texture& tex);
			~TextureCube();

			TextureCube(const TextureCube&) = delete;
			TextureCube(TextureCube&& t);
			TextureCube& operator = (const TextureCube&) = delete;
			TextureCube& operator = (TextureCube&& t);

			GLuint getHandle() const;
			eTextureFormat getFormat() const;

			// [NOTE] these are not particularily safe!
			//void setFace(eCubeFace face, const gli::texture2D& t);
			//void setFace(eCubeFace face, const unsigned char* rawData);

			void bind();
			void unbind();

			void bind(int textureUnit);
			void unbind(int textureUnit);

		private:
			GLuint mHandle;
			eTextureFormat mFormat;
		};

		TextureCube loadTextureCube(const std::string& filepath); // if the file specified is a single image, it'll be applied to all faces
		TextureCube loadTextureCube(
			const std::string& positiveX,
			const std::string& negativeX,
			const std::string& positiveY,
			const std::string& negativeY,
			const std::string& positiveZ,
			const std::string& negativeZ
		);
	}
}
