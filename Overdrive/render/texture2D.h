#pragma once

#include "../opengl.h"
#include "texture.h"

namespace overdrive {
	namespace render {
		// http://docs.gl/gl4/glGenTextures
		// http://docs.gl/gl4/glBindTexture
		// http://docs.gl/gl4/glBindTextureUnit
		// http://docs.gl/gl4/glTexImage2D
		// http://docs.gl/gl4/glTexParameter (not all parameters have an interface yet)
		class Texture2D {
		public:
			Texture2D(eTextureFormat fmt, GLsizei width, GLsizei height);

			template <typename T>
			Texture2D(eTextureFormat fmt, GLsizei width, GLsizei height, T* rawData);

			~Texture2D();

			Texture2D(const Texture2D&) = delete;
			Texture2D(Texture2D&&) = delete;
			Texture2D& operator = (const Texture2D&) = delete;
			Texture2D& operator = (Texture2D&&) = delete;

			GLuint getHandle() const;

			eTextureFormat getFormat() const;
			GLsizei getWidth() const;
			GLsizei getHeight() const;

			template <typename T>
			void loadRawData(T* data);

			void bind();
			void unbind();

			void bind(GLuint unit);
			void unbind(GLuint unit);

			void generateMipmap();

			void setFilter(eTextureMinFilter minification);
			void setFilter(eTextureMagFilter magnification);
			eTextureMinFilter getMinFilter() const;
			eTextureMagFilter getMagFilter() const;

			void setWrap(eTextureWrapping s, eTextureWrapping t);
			eTextureWrapping getWrapS() const;
			eTextureWrapping getWrapT() const;

		private:
			GLuint mHandle;
			
			eTextureFormat mFormat;
			GLsizei mWidth;
			GLsizei mHeight;

			eTextureMinFilter mMinFilter;
			eTextureMagFilter mMagFilter;
			eTextureWrapping mWrappingS;
			eTextureWrapping mWrappingT;
		};
	}
}

#include "texture2d.inl"