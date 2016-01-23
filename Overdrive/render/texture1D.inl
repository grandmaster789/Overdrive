#pragma once

#include "texture1D.h"

namespace overdrive {
	namespace render {
		template <typename T>
		Texture1D::Texture1D(
			eTextureFormat fmt,
			GLsizei width,
			T* rawData
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
				rawData						// source data
			);

			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

			glBindTexture(GL_TEXTURE_1D, 0);
		}

		template <typename T>
		void Texture1D::loadRawData(T* data) {
			GLint internalFormat = static_cast<GLint>(mFormat);
			GLenum pixelFormat = static_cast<GLenum>(getDefaultPixelFormat(mFormat));
			GLenum pixelDataType = GL_UNSIGNED_BYTE;

			bind();

			glTexImage1D(
				GL_TEXTURE_1D,
				0,
				internalFormat,
				mWidth,
				mHeight,
				0,
				pixelFormat,
				pixelDataType,
				data
			);

			unbind();
		}
	}
}