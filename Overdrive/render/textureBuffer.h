#pragma once

#include "../opengl.h"
#include "texture.h"

namespace overdrive {
	namespace render {
		// http://docs.gl/gl4/glTexBuffer
		class TextureBuffer {
		public:
			TextureBuffer(eTextureBufferFormat fmt);
			TextureBuffer(eTextureBufferFormat fmt, GLuint buffer);
			~TextureBuffer();

			TextureBuffer(const TextureBuffer&) = delete;
			TextureBuffer(TextureBuffer&&) = delete;
			TextureBuffer& operator = (const TextureBuffer&) = delete;
			TextureBuffer& operator = (TextureBuffer&&) = delete;

			GLuint getHandle() const;

			eTextureBufferFormat getFormat() const;

			void attach(GLuint buffer);
			void detach();

			void bind();
			void unbind();

			void bind(GLuint unit);
			void unbind(GLuint unit);

		private:
			eTextureBufferFormat mFormat;
			GLuint mHandle;
		};
	}
}
