#pragma once

#include "../opengl.h"
#include "texture.h"

namespace overdrive {
	namespace render {
		// http://docs.gl/gl4/glTexImage1D

		class Texture1D {
		public:
			Texture1D(eTextureFormat fmt, GLsizei width);
			~Texture1D();

			Texture1D(const Texture1D&) = delete;
			Texture1D(Texture1D&&) = delete;
			Texture1D& operator = (const Texture1D&) = delete;
			Texture1D& operator = (Texture1D&&) = delete;

			GLuint getHandle() const;

			eTextureFormat getFormat() const;
			GLsizei getWidth() const;

			void bind();
			void unbind();

			void bind(GLuint unit);
			void unbind(GLuint unit);

			void generateMipmap();

			void setFilter(eTextureMinFilter minification);
			void setFilter(eTextureMagFilter magnification);
			eTextureMinFilter getMinFilter() const;
			eTextureMagFilter getMagFilter() const;

			void setWrap(eTextureWrapping s);
			eTextureWrapping getWrapS() const;

		private:
			GLuint mHandle;

			eTextureFormat mFormat;
			GLsizei mWidth;

			eTextureMinFilter mMinFilter;
			eTextureMagFilter mMagFilter;
			eTextureWrapping mWrappingS;
		};
	}
}
