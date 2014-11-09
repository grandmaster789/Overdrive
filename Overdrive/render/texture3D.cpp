#include "render/texture3d.h"
#include "util/checkGL.h"

namespace overdrive {
	namespace render {
		Texture3D::Texture3D():
			Texture()
		{
		}

		Texture3D::Texture3D(
			const void* data,
			unsigned int imageWidth,
			unsigned int imageHeight,
			unsigned int imageDepth,
			eFormat format,
			eDataType dataType,
			eInternalFormat internalFormat
		):
			Texture(format, dataType, internalFormat)
		{
			TexGuard guard;

			glBindTexture(GL_TEXTURE_3D, mHandle);

			// https://www.opengl.org/sdk/docs/man/html/glTexImage3D.xhtml
			glTexImage3D(
				GL_TEXTURE_3D,							// target
				0,										// mipmap level
				static_cast<GLint>(mInternalFormat),	// internal format
				imageWidth,								// texture width
				imageHeight,							// texture height
				imageDepth,								// texture depth
				0,										// border (must always be 0?)
				static_cast<GLenum>(mFormat),			// format
				static_cast<GLenum>(mDataType),			// data type
				data									// pointer to actual pixeldata
			);

			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glGenerateMipmap(GL_TEXTURE_3D);

			CHECK_GL_STATE;
		}

		void Texture3D::bind() {
			glBindTexture(GL_TEXTURE_3D, mHandle);
		}

		void Texture3D::setWrapping(eWrapping s, eWrapping t, eWrapping r) {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_3D, mHandle);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, static_cast<GLint>(r));
		}

		void Texture3D::setFilters(eFilter mini, eFilter mag) {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_3D, mHandle);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(mini));
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
		}

		void Texture3D::setBorder(const util::Color& c) {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_3D, mHandle);

			float rgba[4] = {
				c.mRed,
				c.mGreen,
				c.mBlue,
				c.mAlpha
			};

			glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, rgba);
		}

		void Texture3D::generateMipmaps() {
			TexGuard guard;

			glBindTexture(GL_TEXTURE_3D, mHandle);
			glGenerateMipmap(GL_TEXTURE_3D);
		}

		Texture3D::TexGuard::TexGuard() :
			mCurrentTextureBinding(0)
		{
			glGetIntegerv(GL_TEXTURE_BINDING_3D, &mCurrentTextureBinding);
		}

		Texture3D::TexGuard::~TexGuard() {
			if (mCurrentTextureBinding)
				glBindTexture(GL_TEXTURE_3D, mCurrentTextureBinding);
		}
	}
}