#include "render/renderbuffer.h"

namespace overdrive {
	namespace render {
		RenderBuffer::RenderBuffer() {
			glGenRenderbuffers(1, &mHandle);
			assert(mHandle != 0);
		}

		RenderBuffer::RenderBuffer(
			unsigned int width, 
			unsigned int height, 
			Texture::eInternalFormat format
		):
			RenderBuffer()
		{
			setStorage(width, height, format);
		}

		RenderBuffer::~RenderBuffer() {
			if (mHandle)
				glDeleteRenderbuffers(1, &mHandle);
		}

		void RenderBuffer::bind() {
			glBindBuffer(GL_RENDERBUFFER, mHandle);
		}

		void RenderBuffer::setStorage(
			unsigned int width,
			unsigned int height,
			Texture::eInternalFormat internalFormat
		) {
			glBindBuffer(GL_RENDERBUFFER, mHandle);
			glRenderbufferStorage(
				GL_RENDERBUFFER, 
				static_cast<GLint>(internalFormat), 
				width, 
				height
			);
		}

		GLuint RenderBuffer::getHandle() const {
			return mHandle;
		}
	}
}
