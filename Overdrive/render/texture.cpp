#include "render/texture.h"

namespace overdrive {
	namespace render {
		Texture::Texture(
			eFormat format,
			eDataType dataType,
			eInternalFormat internalFormat
		):
			mFormat(format),
			mDataType(dataType),
			mInternalFormat(internalFormat)
		{
			glGenTextures(1, &mHandle);
			assert(mHandle != 0);
		}

		Texture::~Texture() {
			glDeleteTextures(1, &mHandle);
		}

		GLuint Texture::getHandle() const {
			return mHandle;
		}

		Texture2D::Texture2D(
			const void* data,
			unsigned int imageWidth,
			unsigned int imageHeight,
			eFormat format,
			eDataType dataType,
			eInternalFormat internalFormat
		):
			Texture(format, dataType, internalFormat)
		{
			glTexImage2D(
				GL_TEXTURE_2D, 
				0, 
				static_cast<GLint>(mInternalFormat), 
				imageWidth, 
				imageHeight,
				0,
				static_cast<GLenum>(mFormat),
				static_cast<GLenum>(mDataType),
				data
			);
		}
	}
}