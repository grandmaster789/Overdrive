#include "stdafx.h"
#include "textureBuffer.h"

namespace overdrive {
	namespace render {
		TextureBuffer::TextureBuffer(eTextureBufferFormat fmt):
			mFormat(fmt)
		{
			glGenTextures(1, &mHandle);
		}

		TextureBuffer::TextureBuffer(eTextureBufferFormat fmt, GLuint buffer):
			mFormat(fmt)
		{
			GLenum format = static_cast<GLenum>(fmt);

			glGenTextures(1, &mHandle);
			glBindTexture(GL_TEXTURE_BUFFER, mHandle);
			glTexBuffer(GL_TEXTURE_BUFFER, format, buffer);
		}

		TextureBuffer::~TextureBuffer() {
			if (mHandle)
				glDeleteTextures(1, &mHandle);
		}

		GLuint TextureBuffer::getHandle() const {
			return mHandle;
		}

		eTextureBufferFormat TextureBuffer::getFormat() const {
			return mFormat;
		}

		void TextureBuffer::attach(GLuint buffer) {
			GLenum format = static_cast<GLenum>(mFormat);

			glBindTexture(GL_TEXTURE_BUFFER, mHandle);
			glTexBuffer(GL_TEXTURE_BUFFER, format, buffer);
		}

		void TextureBuffer::detach() {
			GLenum format = static_cast<GLenum>(mFormat);

			glBindTexture(GL_TEXTURE_BUFFER, mHandle);
			glTexBuffer(GL_TEXTURE_BUFFER, format, 0);
		}

		void TextureBuffer::bind() {
			glBindTexture(GL_TEXTURE_BUFFER, mHandle);
		}

		void TextureBuffer::unbind() {
			glBindTexture(GL_TEXTURE_BUFFER, 0);
		}

		void TextureBuffer::bind(GLuint unit) {
			glBindTextureUnit(unit, mHandle);
		}

		void TextureBuffer::unbind(GLuint unit) {
			glBindTextureUnit(unit, 0);
		}
	}
}