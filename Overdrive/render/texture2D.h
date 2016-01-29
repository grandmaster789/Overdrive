#pragma once

#include "texture.h"

namespace overdrive {
	namespace render {
		class Texture2D {
		public:
			Texture2D(eTextureFormat format, int width, int height);
			Texture2D(eTextureFormat format, int width, int height, unsigned char* rawData, size_t numBytes); // assumes 1 mip level, the simplest texture possible
			Texture2D(const std::string& filepath); // probably should be a full path
			~Texture2D();
			
			Texture2D(const Texture2D&) = delete;
			Texture2D(Texture2D&&) = delete;
			Texture2D& operator = (const Texture2D&) = delete;
			Texture2D& operator = (Texture2D&&) = delete;

			GLuint getHandle() const;
			eTextureFormat getFormat() const;
			eTextureFormatType getFormatType() const;

			void setWrapS(eWrapping mode);
			void setWrapT(eWrapping mode);
			eWrapping getWrapS() const;
			eWrapping getWrapT() const;

			void setFilter(eMinFilter minification);
			void setFilter(eMagFilter magnification);
			eMinFilter getMinFilter() const;
			eMagFilter getMagFilter() const;

			void setSwizzleR(eSwizzle mode);
			void setSwizzleG(eSwizzle mode);
			void setSwizzleB(eSwizzle mode);
			void setSwizzleA(eSwizzle mode);
			void setSwizzleRGBA(eSwizzle mode);
			eSwizzle getSwizzleR() const;
			eSwizzle getSwizzleG() const;
			eSwizzle getSwizzleB() const;
			eSwizzle getSwizzleA() const;

			void bind();
			void unbind();

			void bind(int textureUnit);
			void unbind(int textureUnit);

		private:
			GLuint mHandle;

			std::unique_ptr<gli::texture2D> mData; // [NOTE] egh... the extra indirection is here so we can dynamically determine sizes and formats used
			static gli::gl mConverter; // contains translations from gli constants to openGL constants

			int mTextureBaseLevel;
			int mTextureMaxLevel;

			eWrapping mWrapS;
			eWrapping mWrapT;

			eMinFilter mMinFilter;
			eMagFilter mMagFilter;

			eSwizzle mSwizzleR;
			eSwizzle mSwizzleG;
			eSwizzle mSwizzleB;
			eSwizzle mSwizzleA;
		};
	}
}
