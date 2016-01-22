#pragma once

#include "../opengl.h"
#include "texture.h"

namespace overdrive {
	namespace render {
		// http://docs.gl/gl4/glGenTextures
		// http://docs.gl/gl4/glBindTexture
		// http://docs.gl/gl4/glBindTextureUnit
		// http://docs.gl/gl4/glTexImage3D
		// http://docs.gl/gl4/glTexParameter (not all parameters have an interface yet)
		// http://docs.gl/gl4/glGenerateMipmap
		class Texture2DArray {
		public:
			Texture2DArray(eTextureFormat fmt, GLsizei width, GLsizei height, GLsizei depth);
			~Texture2DArray();

			Texture2DArray(const Texture2DArray&) = delete;
			Texture2DArray(Texture2DArray&&) = delete;
			Texture2DArray& operator = (const Texture2DArray&) = delete;
			Texture2DArray& operator = (Texture2DArray&&) = delete;

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

			void setWrap(eTextureWrapping s, eTextureWrapping t);
			eTextureWrapping getWrapS() const;
			eTextureWrapping getWrapT() const;

			void setCompareMode(eTextureCompareMode mode);
			void setCompareFunction(eTextureCompareFunction func);
			eTextureCompareMode getCompareMode() const;
			eTextureCompareFunction getCompareFunction() const;

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

			eTextureCompareMode mCompareMode;
			eTextureCompareFunction mCompareFunction;
		};
	}
}