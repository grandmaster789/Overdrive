#include "render/texture2D.h"
#include "util/checkGL.h"

namespace overdrive {
	namespace render {
		Texture2D::Texture2D():
			Texture()
		{
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
			TexGuard guard;

			glBindTexture(GL_TEXTURE_2D, mHandle);

			// https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
			glTexImage2D(
				GL_TEXTURE_2D,							// target
				0,										// mipmap level
				static_cast<GLint>(mInternalFormat),	// internal format
				imageWidth,								// texture width
				imageHeight,							// texture height
				0,										// border (must always be 0?)
				static_cast<GLenum>(mFormat),			// format
				static_cast<GLenum>(mDataType),			// data type
				data									// pointer to actual pixeldata
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glGenerateMipmap(GL_TEXTURE_2D);

			CHECK_GL_STATE;
		}

		void Texture2D::bind() {
			glBindTexture(GL_TEXTURE_2D, mHandle);
		}

		void Texture2D::setWrapping(eWrapping s, eWrapping t) {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_2D, mHandle);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
		}

		void Texture2D::setFilters(eFilter mini, eFilter mag) {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_2D, mHandle);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(mini));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
		}

		void Texture2D::setBorder(const util::Color& c) {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_2D, mHandle);

			float rgba[4] = {
				c.mRed,
				c.mGreen,
				c.mBlue,
				c.mAlpha
			};

			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, rgba);
		}

		void Texture2D::generateMipmaps() {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_2D, mHandle);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		Texture2D::TexGuard::TexGuard() :
			mCurrentTextureBinding(0)
		{
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &mCurrentTextureBinding);
		}

		Texture2D::TexGuard::~TexGuard() {
			if (mCurrentTextureBinding)
				glBindTexture(GL_TEXTURE_2D, mCurrentTextureBinding);
		}
	}
}
