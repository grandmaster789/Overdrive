#include "stdafx.h"
#include "texture2Dmultisample.h"

namespace overdrive {
	namespace render {
		Texture2DMultisample::Texture2DMultisample(
			eTextureFormat fmt,
			GLsizei width,
			GLsizei height,
			GLsizei numSamples
		):
			mFormat(fmt),
			mWidth(width),
			mHeight(height),
			mNumSamples(numSamples)
		{
			GLenum internalFormat = static_cast<GLenum>(fmt);

			glGenTextures(1, &mHandle);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mHandle);

			glTexImage2DMultisample(
				GL_TEXTURE_2D_MULTISAMPLE,
				numSamples,
				internalFormat,
				width,
				height,
				GL_FALSE	// fixed sample locations
			);

			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0); // is this appropriate?
		}

		Texture2DMultisample::~Texture2DMultisample() {
			if (mHandle)
				glDeleteTextures(1, &mHandle);
		}

		GLuint Texture2DMultisample::getHandle() const {
			return mHandle;
		}

		eTextureFormat Texture2DMultisample::getFormat() const {
			return mFormat;
		}

		GLsizei Texture2DMultisample::getWidth() const {
			return mWidth;
		}

		GLsizei Texture2DMultisample::getHeight() const {
			return mHeight;
		}

		GLsizei Texture2DMultisample::getNumSamples() const {
			return mNumSamples;
		}

		void Texture2DMultisample::bind() {
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mHandle);
		}

		void Texture2DMultisample::unbind() {
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		}

		void Texture2DMultisample::bind(GLuint unit) {
			glBindTextureUnit(unit, mHandle);
		}

		void Texture2DMultisample::unbind(GLuint unit) {
			glBindTextureUnit(unit, 0);
		}
	}
}