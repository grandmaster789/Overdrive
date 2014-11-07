#include "texture1D.h"
#include "util/checkGL.h"

namespace overdrive {
	namespace render {
		Texture1D::Texture1D(
			const void* data,
			unsigned int imageSize,
			eFormat format,
			eDataType dataType,
			eInternalFormat internalFormat
			) :
			Texture(format, dataType, internalFormat)
		{
			TexGuard guard;

			glBindTexture(GL_TEXTURE_1D, mHandle);

			// https://www.opengl.org/sdk/docs/man/html/glTexImage1D.xhtml
			glTexImage1D(
				GL_TEXTURE_1D,							// target
				0,										// mipmap level
				static_cast<GLint>(mInternalFormat),	// internal format
				imageSize,								// texture width
				0,										// border (must always be 0?)
				static_cast<GLenum>(mFormat),			// format
				static_cast<GLenum>(mDataType),			// data type
				data									// pointer to actual pixeldata
				);

			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glGenerateMipmap(GL_TEXTURE_1D);

			CHECK_GL_STATE;
		}

		void Texture1D::setWrapping(eWrapping s) {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_1D, mHandle);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
		}

		void Texture1D::setFilters(eFilter mini, eFilter mag) {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_1D, mHandle);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(mini));
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
		}

		void Texture1D::setBorder(const util::Color& c) {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_1D, mHandle);

			float rgba[4] = {
				c.mRed,
				c.mGreen,
				c.mBlue,
				c.mAlpha
			};

			glTexParameterfv(GL_TEXTURE_1D, GL_TEXTURE_BORDER_COLOR, rgba);
		}

		void Texture1D::generateMipmaps() {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_1D, mHandle);
			glGenerateMipmap(GL_TEXTURE_1D);
		}

		Texture1D::TexGuard::TexGuard() :
			mCurrentTextureBinding(0)
		{
			glGetIntegerv(GL_TEXTURE_BINDING_1D, &mCurrentTextureBinding);
		}

		Texture1D::TexGuard::~TexGuard() {
			if (mCurrentTextureBinding)
				glBindTexture(GL_TEXTURE_1D, mCurrentTextureBinding);
		}
	}
}