#include "stdafx.h"
#include "texture2D.h"

namespace overdrive {
	namespace render {
		Texture2D::Texture2D(
			eTextureFormat fmt, 
			GLsizei width, 
			GLsizei height
		):
			mFormat(fmt),
			mWidth(width),
			mHeight(height),

			mMinFilter(eTextureMinFilter::LINEAR),
			mMagFilter(eTextureMagFilter::LINEAR),
			mWrappingS(eTextureWrapping::REPEAT),
			mWrappingT(eTextureWrapping::REPEAT)
		{
			GLint internalFormat = static_cast<GLint>(fmt);
			GLenum pixelFormat = static_cast<GLenum>(getDefaultPixelFormat(fmt));
			GLenum pixelDataType = GL_UNSIGNED_BYTE;
			
			glGenTextures(1, &mHandle);
			glBindTexture(GL_TEXTURE_2D, mHandle);
			
			glTexImage2D(
				GL_TEXTURE_2D,				// target
				0,							// mipmap level
				internalFormat,				// internal format
				width,						// width
				height,						// height
				0,							// border (must be 0)
				pixelFormat,				// pixel format
				pixelDataType,				// pixel data type
				nullptr						// source data
			);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glBindTexture(GL_TEXTURE_2D, 0); // is this appropriate?
		}

		Texture2D::~Texture2D() {
			if (mHandle)
				glDeleteTextures(1, &mHandle);
		}

		GLuint Texture2D::getHandle() const {
			return mHandle;
		}

		eTextureFormat Texture2D::getFormat() const {
			return mFormat;
		}

		GLsizei Texture2D::getWidth() const {
			return mWidth;
		}

		GLsizei Texture2D::getHeight() const {
			return mHeight;
		}

		void Texture2D::bind() {
			glBindTexture(GL_TEXTURE_2D, mHandle);
		}

		void Texture2D::unbind() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture2D::bind(GLuint unit) {
			glBindTextureUnit(unit, mHandle);
		}

		void Texture2D::unbind(GLuint unit) {
			glBindTextureUnit(unit, 0);
		}

		void Texture2D::generateMipmap() {
			bind();
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			mMinFilter = eTextureMinFilter::LINEAR_MIPMAP_LINEAR;
		}

		void Texture2D::setFilter(eTextureMinFilter minification) {
			if (minification != mMinFilter) {
				bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minification));
				mMinFilter = minification;
			}
		}

		void Texture2D::setFilter(eTextureMagFilter magnification) {
			if (magnification != mMagFilter) {
				bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magnification));
				mMagFilter = magnification;
			}
		}

		eTextureMinFilter Texture2D::getMinFilter() const {
			return mMinFilter;
		}

		eTextureMagFilter Texture2D::getMagFilter() const {
			return mMagFilter;
		}

		void Texture2D::setWrap(eTextureWrapping s, eTextureWrapping t) {
			if (mWrappingS != s) {
				bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
				mWrappingS = s;
			}

			if (mWrappingT != t) {
				bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
				mWrappingT = t;
			}
		}

		eTextureWrapping Texture2D::getWrapS() const {
			return mWrappingS;
		}

		eTextureWrapping Texture2D::getWrapT() const {
			return mWrappingT;
		}
	}
}
