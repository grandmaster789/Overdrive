#include "stdafx.h"
#include "texture1D.h"

namespace overdrive {
	namespace render {
		Texture1D::Texture1D(
			eTextureFormat fmt,
			GLsizei width
		):
			mFormat(fmt),
			mWidth(width),

			mMinFilter(eTextureMinFilter::LINEAR),
			mMagFilter(eTextureMagFilter::LINEAR),
			mWrappingS(eTextureWrapping::REPEAT)
		{
			GLint internalFormat = static_cast<GLint>(fmt);
			GLenum pixelFormat = static_cast<GLenum>(getDefaultPixelFormat(fmt));
			GLenum pixelDataType = GL_UNSIGNED_BYTE;

			glGenTextures(1, &mHandle);
			glBindTexture(GL_TEXTURE_1D, mHandle);

			glTexImage1D(
				GL_TEXTURE_1D,				// target
				0,							// mipmap level
				internalFormat,				// internal format
				width,						// width
				0,							// border (must be 0)
				pixelFormat,				// pixel format
				pixelDataType,				// pixel data type
				nullptr						// source data
			);

			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

			glBindTexture(GL_TEXTURE_1D, 0); // is this appropriate?
		}

		Texture1D::~Texture1D() {
			if (mHandle)
				glDeleteTextures(1, &mHandle);
		}

		GLuint Texture1D::getHandle() const {
			return mHandle;
		}

		eTextureFormat Texture1D::getFormat() const {
			return mFormat;
		}

		GLsizei Texture1D::getWidth() const {
			return mWidth;
		}

		void Texture1D::bind() {
			glBindTexture(GL_TEXTURE_1D, mHandle);
		}

		void Texture1D::unbind() {
			glBindTexture(GL_TEXTURE_1D, 0);
		}

		void Texture1D::bind(GLuint unit) {
			glBindTextureUnit(unit, mHandle);
		}

		void Texture1D::unbind(GLuint unit) {
			glBindTextureUnit(unit, 0);
		}

		void Texture1D::generateMipmap() {
			bind();
			glGenerateMipmap(GL_TEXTURE_1D);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			mMinFilter = eTextureMinFilter::LINEAR_MIPMAP_LINEAR;
		}

		void Texture1D::setFilter(eTextureMinFilter minification) {
			if (minification != mMinFilter) {
				bind();
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minification));
				mMinFilter = minification;
			}
		}

		void Texture1D::setFilter(eTextureMagFilter magnification) {
			if (magnification != mMagFilter) {
				bind();
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magnification));
				mMagFilter = magnification;
			}
		}

		eTextureMinFilter Texture1D::getMinFilter() const {
			return mMinFilter;
		}

		eTextureMagFilter Texture1D::getMagFilter() const {
			return mMagFilter;
		}

		void Texture1D::setWrap(eTextureWrapping s) {
			if (mWrappingS != s) {
				bind();
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
				mWrappingS = s;
			}
		}

		eTextureWrapping Texture1D::getWrapS() const {
			return mWrappingS;
		}
	}
}
