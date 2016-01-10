#pragma once

#include "../opengl.h"
#include "texture.h"

namespace overdrive {
	namespace render {
		// http://docs.gl/gl4/glTexImage2DMultisample
		class Texture2DMultisample {
		public:
			Texture2DMultisample(eTextureFormat fmt, GLsizei width, GLsizei height, GLsizei numSamples);
			~Texture2DMultisample();

			Texture2DMultisample(const Texture2DMultisample&) = delete;
			Texture2DMultisample(Texture2DMultisample&&) = delete;
			Texture2DMultisample& operator = (const Texture2DMultisample&) = delete;
			Texture2DMultisample& operator = (Texture2DMultisample&&) = delete;

			GLuint getHandle() const;

			eTextureFormat getFormat() const;
			GLsizei getWidth() const;
			GLsizei getHeight() const;
			GLsizei getNumSamples() const;

			void bind();
			void unbind();

			void bind(GLuint unit);
			void unbind(GLuint unit);

		private:
			GLuint mHandle;

			eTextureFormat mFormat;
			GLsizei mWidth;
			GLsizei mHeight;
			GLsizei mNumSamples;
		};
	}
}
