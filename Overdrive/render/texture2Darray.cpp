#include "stdafx.h"
#include "texture2Darray.h"

namespace overdrive {
	namespace render {
		Texture2DArray::Texture2DArray(
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
			mCompareMode(eTextureCompareMode::NONE),
			mCompareFunction(eTextureCompareFunction::LESS)
		{
			GLint internalFormat = static_cast<GLint>(fmt);
			GLenum pixelFormat = static_cast<GLenum>(getDefaultPixelFormat(fmt));
			GLenum pixelDataType = GL_UNSIGNED_BYTE;

			glGenTextures(1, &mHandle);
			glBindTexture(GL_TEXTURE_2D_ARRAY, mHandle);

			glTexImage3D(
				GL_TEXTURE_2D_ARRAY,		// target
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

			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glBindTexture(GL_TEXTURE_2D_ARRAY, 0); // is this appropriate?
		}

		Texture2DArray::~Texture2DArray() {
			if (mHandle)
				glDeleteTextures(1, &mHandle);
		}

		GLuint Texture2DArray::getHandle() const {
			return mHandle;
		}

		eTextureFormat Texture2DArray::getFormat() const {
			return mFormat;
		}

		GLsizei Texture2DArray::getWidth() const {
			return mWidth;
		}

		GLsizei Texture2DArray::getHeight() const {
			return mHeight;
		}

		void Texture2DArray::bind() {
			glBindTexture(GL_TEXTURE_2D_ARRAY, mHandle);
		}

		void Texture2DArray::unbind() {
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		}

		void Texture2DArray::bind(GLuint unit) {
			glBindTextureUnit(unit, mHandle);
		}

		void Texture2DArray::unbind(GLuint unit) {
			glBindTextureUnit(unit, 0);
		}

		void Texture2DArray::generateMipmap() {
			bind();
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			mMinFilter = eTextureMinFilter::LINEAR_MIPMAP_LINEAR;
		}

		void Texture2DArray::setFilter(eTextureMinFilter minification) {
			if (minification != mMinFilter) {
				bind();
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minification));
				mMinFilter = minification;
			}
		}

		void Texture2DArray::setFilter(eTextureMagFilter magnification) {
			if (magnification != mMagFilter) {
				bind();
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magnification));
				mMagFilter = magnification;
			}
		}

		eTextureMinFilter Texture2DArray::getMinFilter() const {
			return mMinFilter;
		}

		eTextureMagFilter Texture2DArray::getMagFilter() const {
			return mMagFilter;
		}

		void Texture2DArray::setWrap(
			eTextureWrapping s,
			eTextureWrapping t
		) {
			if (mWrappingS != s) {
				bind();
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
				mWrappingS = s;
			}

			if (mWrappingT != t) {
				bind();
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
				mWrappingT = t;
			}
		}

		eTextureWrapping Texture2DArray::getWrapS() const {
			return mWrappingS;
		}

		eTextureWrapping Texture2DArray::getWrapT() const {
			return mWrappingT;
		}

		void Texture2DArray::setCompareMode(eTextureCompareMode mode) {
			if (mCompareMode != mode) {
				bind();
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, static_cast<GLenum>(mode));
				mCompareMode = mode;
			}
		}

		void Texture2DArray::setCompareFunction(eTextureCompareFunction func) {
			if (mCompareFunction != func) {
				bind();
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, static_cast<GLenum>(func));
				mCompareFunction = func;
			}
		}

		eTextureCompareMode Texture2DArray::getCompareMode() const {
			return mCompareMode;
		}

		eTextureCompareFunction Texture2DArray::getCompareFunction() const {
			return mCompareFunction;
		}
	}
}
