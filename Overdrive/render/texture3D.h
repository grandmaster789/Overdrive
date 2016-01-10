#pragma once

#include "../opengl.h"
#include "texture.h"

namespace overdrive {
	namespace render {
		// [NOTE] there doesn't appear to be a GL_TEXTURE_3D_MULTISAMPLE target, so I'm not going to provide one

		// http://docs.gl/gl4/glGenTextures
		// http://docs.gl/gl4/glBindTexture
		// http://docs.gl/gl4/glBindTextureUnit
		// http://docs.gl/gl4/glTexImage3D
		// http://docs.gl/gl4/glTexParameter (not all parameters have an interface yet)
		// http://docs.gl/gl4/glGenerateMipmap
		class Texture3D {
		public:
			Texture3D(eTextureFormat fmt, GLsizei width, GLsizei height, GLsizei depth);
			~Texture3D();

			Texture3D(const Texture3D&) = delete;
			Texture3D(Texture3D&&) = delete;
			Texture3D& operator = (const Texture3D&) = delete;
			Texture3D& operator = (Texture3D&&) = delete;

			GLuint getHandle() const;

			eTextureFormat getFormat() const;
			GLsizei getWidth() const;
			GLsizei getHeight() const;

			void bind();
			void unbind();

			void bind(GLuint unit);
			void unbind(GLuint unit);

			void generateMipmap();

			void setFilter(eTextureMinFilter minification);
			void setFilter(eTextureMagFilter magnification);
			eTextureMinFilter getMinFilter() const;
			eTextureMagFilter getMagFilter() const;

			void setWrap(eTextureWrapping s, eTextureWrapping t, eTextureWrapping r);
			eTextureWrapping getWrapS() const;
			eTextureWrapping getWrapT() const;
			eTextureWrapping getWrapR() const;

		private:
			GLuint mHandle;

			eTextureFormat mFormat;
			GLsizei mWidth;
			GLsizei mHeight;
			GLsizei mDepth;
			
			eTextureMinFilter mMinFilter;
			eTextureMagFilter mMagFilter;
			eTextureWrapping mWrappingS;
			eTextureWrapping mWrappingT;
			eTextureWrapping mWrappingR;
		};
	}
}