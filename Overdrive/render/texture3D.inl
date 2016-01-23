#pragma once

#include "texture3D.h"

namespace overdrive {
	namespace render {
		template <typename T>
		Texture3D::Texture3D(
			eTextureFormat fmt,
			GLsizei width,
			GLsizei height,
			GLsizei depth,
			T* rawData
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
				depth,
				0,							// border (must be 0)
				pixelFormat,				// pixel format
				pixelDataType,				// pixel data type
				rawData						// source data
			);

			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

			glBindTexture(GL_TEXTURE_3D, 0);
		}

		template <typename T>
		void Texture3D::loadRawData(T* data) {
			GLint internalFormat = static_cast<GLint>(mFormat);
			GLenum pixelFormat = static_cast<GLenum>(getDefaultPixelFormat(mFormat));
			GLenum pixelDataType = GL_UNSIGNED_BYTE;

			bind();

			glTexImage3D(
				GL_TEXTURE_3D,
				0,
				internalFormat,
				mWidth,
				mHeight,
				mDepth,
				0,
				pixelFormat,
				pixelDataType,
				data
			);

			unbind();
		}
	}
}