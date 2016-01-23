#include "stdafx.h"
#include "texture3D.h"

namespace overdrive {
	namespace render {
		Texture3D::Texture3D(
			eTextureFormat fmt,
			GLsizei width,
			GLsizei height,
			GLsizei depth
		):
			mFormat(fmt),
			mWidth(width),
			mHeight(height),
			mDepth(depth),

			mMinFilter(eTextureMinFilter::LINEAR),
			mMagFilter(eTextureMagFilter::LINEAR),
			mWrappingS(eTextureWrapping::REPEAT),
			mWrappingT(eTextureWrapping::REPEAT),
			mWrappingR(eTextureWrapping::REPEAT)
		{
			GLint internalFormat = static_cast<GLint>(fmt);
			GLenum pixelFormat = static_cast<GLenum>(getDefaultPixelFormat(fmt));
			GLenum pixelDataType = GL_UNSIGNED_BYTE;

			glGenTextures(1, &mHandle);
			glBindTexture(GL_TEXTURE_3D, mHandle);

			glTexImage3D(
				GL_TEXTURE_3D,				// target
				0,							// mipmap level
				internalFormat,				// internal format
				width,						// width
				height,						// height
				depth,						// depth
				0,							// border (must be 0)
				pixelFormat,				// pixel format
				pixelDataType,				// pixel data type
				nullptr						// source data
			);

			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

			glBindTexture(GL_TEXTURE_3D, 0); // is this appropriate?
		}

		Texture3D::~Texture3D() {
			if (mHandle)
				glDeleteTextures(1, &mHandle);
		}

		GLuint Texture3D::getHandle() const {
			return mHandle;
		}

		eTextureFormat Texture3D::getFormat() const {
			return mFormat;
		}

		GLsizei Texture3D::getWidth() const {
			return mWidth;
		}

		GLsizei Texture3D::getHeight() const {
			return mHeight;
		}

		GLsizei Texture3D::getDepth() const {
			return mDepth;
		}

		void Texture3D::bind() {
			glBindTexture(GL_TEXTURE_3D, mHandle);
		}

		void Texture3D::unbind() {
			glBindTexture(GL_TEXTURE_3D, 0);
		}

		void Texture3D::bind(GLuint unit) {
			glBindTextureUnit(unit, mHandle);
		}

		void Texture3D::unbind(GLuint unit) {
			glBindTextureUnit(unit, 0);
		}

		void Texture3D::generateMipmap() {
			bind();
			glGenerateMipmap(GL_TEXTURE_3D);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			mMinFilter = eTextureMinFilter::LINEAR_MIPMAP_LINEAR;
		}

		void Texture3D::setFilter(eTextureMinFilter minification) {
			if (minification != mMinFilter) {
				bind();
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minification));
				mMinFilter = minification;
			}
		}

		void Texture3D::setFilter(eTextureMagFilter magnification) {
			if (magnification != mMagFilter) {
				bind();
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magnification));
				mMagFilter = magnification;
			}
		}

		eTextureMinFilter Texture3D::getMinFilter() const {
			return mMinFilter;
		}

		eTextureMagFilter Texture3D::getMagFilter() const {
			return mMagFilter;
		}

		void Texture3D::setWrap(
			eTextureWrapping s, 
			eTextureWrapping t,
			eTextureWrapping r
		) {
			if (mWrappingS != s) {
				bind();
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
				mWrappingS = s;
			}

			if (mWrappingT != t) {
				bind();
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
				mWrappingT = t;
			}

			if (mWrappingR != r) {
				bind();
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, static_cast<GLint>(r));
				mWrappingR = r;
			}
		}

		eTextureWrapping Texture3D::getWrapS() const {
			return mWrappingS;
		}

		eTextureWrapping Texture3D::getWrapT() const {
			return mWrappingT;
		}

		eTextureWrapping Texture3D::getWrapR() const {
			return mWrappingR;
		}
	}
}
