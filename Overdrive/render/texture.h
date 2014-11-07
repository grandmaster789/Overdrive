#ifndef OVERDRIVE_RENDER_TEXTURE_H
#define OVERDRIVE_RENDER_TEXTURE_H

#include "opengl.h"
#include "util/color.h"
#include <ostream>

namespace overdrive {
	namespace render {
		class Texture {
		public:
			// see https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for more details on the enumeration values
			enum class eFormat: GLenum {
				RED = GL_RED,
				RGB = GL_RGB,
				BGR = GL_BGR,
				RGBA = GL_RGBA,
				BGRA = GL_BGRA
			};

			enum class eInternalFormat : GLint {
				COMPRESSED_RED = GL_COMPRESSED_RED,
				COMPRESSED_RED_RGTC1 = GL_COMPRESSED_RED_RGTC1,
				COMPRESSED_RG = GL_COMPRESSED_RG,
				COMPRESSED_RGB = GL_COMPRESSED_RGB,
				COMPRESSED_RGBA = GL_COMPRESSED_RGBA,
				COMPRESSED_RGRGTC2 = GL_COMPRESSED_RG_RGTC2,
				COMPRESSED_SIGNED_RED_RGTC1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
				COMPRESSED_SIGNED_RG_RGTC2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
				COMPRESSED_SRGB = GL_COMPRESSED_SRGB,

				DEPTH_STENCIL = GL_DEPTH_STENCIL,
				DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
				DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
				DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
				DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
				DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
				DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,

				RED = GL_RED,
				RG = GL_RG,

				RGB = GL_RGB,
				RGB4 = GL_RGB4,
				RGB5 = GL_RGB5,
				RGB5_A1 = GL_RGB5_A1,

				RGBA = GL_RGBA,
				RGBA2 = GL_RGBA2,

				RGBA4 = GL_RGBA4,

				R8 = GL_R8,
				R8I = GL_R8I,
				R8UI = GL_R8UI,
				RG8 = GL_RG8,
				RG8I = GL_RG8I,
				RG8UI = GL_RG8UI,
				RGB8 = GL_RGB8,
				RGB8I = GL_RGB8I,
				RGB8UI = GL_RGB8UI,
				RGBA8 = GL_RGBA8,
				RGBA8UI = GL_RGBA8UI,

				RGB9E5 = GL_RGB9_E5,

				RGB10 = GL_RGB10,
				RGB10A2 = GL_RGB10_A2,

				RGB12 = GL_RGB12,
				RGBA12 = GL_RGBA12,

				R16F = GL_R16F,
				R16I = GL_R16I,
				R16UI = GL_R16UI,
				RG16 = GL_RG16,
				RG16F = GL_RG16F,
				RGB16 = GL_RGB16,
				RGB16F = GL_RGB16F,
				RGB16I = GL_RGB16I,
				RGB16UI = GL_RGB16UI,
				RGBA16 = GL_RGBA16,
				RGBA16F = GL_RGBA16F,
				RGBA16I = GL_RGBA16I,
				RGBA16UI = GL_RGBA16UI,

				R32I = GL_R32I,
				R32F = GL_R32F,
				R32UI = GL_R32UI,
				RG32F = GL_RG32F,
				RG32I = GL_RG32I,
				RG32UI = GL_RG32UI,
				RGB32F = GL_RGB32F,
				RGB32I = GL_RGB32I,
				RGB32UI = GL_RGB32UI,
				RGBA32F = GL_RGBA32F,
				RGBA32I = GL_RGBA32I,
				RGBA32UI = GL_RGBA32UI,

				R3_G3_B2 = GL_R3_G3_B2,

				R8_SNORM = GL_R8_SNORM,
				RG8_SNORM = GL_RG8_SNORM,
				R16_SNORM = GL_R16_SNORM,
				RG16_SNORM = GL_RG16_SNORM,

				SRGB8 = GL_SRGB8,
				SRGB8A8 = GL_SRGB8_ALPHA8,
				SRGBA = GL_SRGB_ALPHA
			};

			enum class eDataType: GLenum {
				BYTE = GL_BYTE,
				SHORT = GL_SHORT,
				INT = GL_INT,

				UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
				UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
				UNSIGNED_INT = GL_UNSIGNED_INT,

				FLOAT = GL_FLOAT,
				DOUBLE = GL_DOUBLE,

				UNSIGNED_BYTE_332 = GL_UNSIGNED_BYTE_3_3_2,
				UNSIGNED_BYTE_233_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
				UNSIGNED_SHORT_565 = GL_UNSIGNED_SHORT_5_6_5,
				UNSIGNED_SHORT_565_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
				UNSIGNED_SHORT_4444 = GL_UNSIGNED_SHORT_4_4_4_4,
				UNSIGNED_SHORT_4444_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
				UNSIGNED_SHORT_5551 = GL_UNSIGNED_SHORT_5_5_5_1,
				UNSIGNED_SHORT_1555_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
				UNSIGNED_INT_8888 = GL_UNSIGNED_INT_8_8_8_8,
				UNSIGNED_INT_8888_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
				UNSIGNED_INT_1010102 = GL_UNSIGNED_INT_10_10_10_2
			};

			enum class eWrapping: GLint {
				CLAMP_EDGE = GL_CLAMP_TO_EDGE,
				CLAMP_BORDER = GL_CLAMP_TO_BORDER,
				REPEAT = GL_REPEAT,
				MIRRORED_REPEAT = GL_MIRRORED_REPEAT
			};

			enum class eFilter: GLenum {
				NEAREST = GL_NEAREST,
				LINEAR = GL_LINEAR,
				NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
				NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
				LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
				LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
			};

			~Texture();

			GLuint getHandle() const;

			eFormat getFormat() const;
			eInternalFormat getInternalFormat() const;
			eDataType getDataType() const;

		protected:
			Texture(
				eFormat format, 
				eDataType dataType, 
				eInternalFormat internalFormat
			);

			Texture(const Texture& t) = delete;
			Texture& operator = (const Texture& t) = delete;

			GLuint mHandle;

			eFormat mFormat;
			eInternalFormat mInternalFormat;
			eDataType mDataType;
		};

		class Texture1D:
			public Texture
		{
		public:
			Texture1D(
				const void* data,
				unsigned int imageSize,
				eFormat format = eFormat::RGBA,
				eDataType dataType = eDataType::UNSIGNED_BYTE,
				eInternalFormat internalFormat = eInternalFormat::RGBA
			);

			void setWrapping(eWrapping s);
			void setFilters(eFilter mini, eFilter mag);
			void setBorder(const util::Color& c);

			void generateMipmaps();

		private:
			// helper class that stores the currently bound texture handle and restores it upon destruction
			class TexGuard {
			public:
				TexGuard();
				TexGuard(const TexGuard&) = delete;
				~TexGuard();

			private:
				GLint mCurrentTextureBinding;
			};
		};

		class Texture2D:
			public Texture
		{
		public:
			Texture2D(
				const void* data,
				unsigned int imageWidth,
				unsigned int imageHeight,
				eFormat format = eFormat::RGBA,
				eDataType dataType = eDataType::UNSIGNED_BYTE,
				eInternalFormat internalFormat = eInternalFormat::RGBA
			);

			void setWrapping(eWrapping s, eWrapping t); // defaults to CLAMP_TO_EDGE
			void setFilters(eFilter mini, eFilter mag);	// defaults to LINEAR_MIPMAP_LINEAR + LINEAR
			void setBorder(const util::Color& c);		// defaults to black

			void generateMipmaps();
		
		private:
			// helper class that stores the currently bound texture handle and restores it upon destruction
			class TexGuard {
			public:
				TexGuard();
				TexGuard(const TexGuard&) = delete;
				~TexGuard();

			private:
				GLint mCurrentTextureBinding;
			};
		};
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::render::Texture::eDataType& datatype);
std::ostream& operator << (std::ostream& os, const overdrive::render::Texture::eFilter& filter);
std::ostream& operator << (std::ostream& os, const overdrive::render::Texture::eFormat& format);
std::ostream& operator << (std::ostream& os, const overdrive::render::Texture::eInternalFormat& format);
std::ostream& operator << (std::ostream& os, const overdrive::render::Texture::eWrapping& wrapping);

#endif
