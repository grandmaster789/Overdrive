#include "stdafx.h"
#include "texture2D.h"
#include "gltypes.h"
#include "../core/logger.h"

namespace overdrive {
	namespace render {
		gli::gl Texture2D::mConverter;

		Texture2D::Texture2D(eTextureFormat format, int width, int height):
			mHandle(0)
		{
			mData = std::make_unique<gli::texture2D>(format, glm::ivec2(width, height));

			glGenTextures(1, &mHandle);

			if (mHandle == 0)
				throw std::runtime_error("Failed to generate a new texture");
		}

		Texture2D::Texture2D(
			eTextureFormat format, 
			int width, 
			int height, 
			unsigned char* rawData, 
			size_t numBytes
		):
			mHandle(0)
		{
			mData = std::make_unique<gli::texture2D>(format, glm::ivec2(width, height));
			memcpy((*mData)[0].data(), rawData, numBytes); // copy the raw data into the object-local storage

			glGenTextures(1, &mHandle);

			if (mHandle == 0)
				throw std::runtime_error("Failed to generate a new texture");

			bind();

			// assume only 1 mip level
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);
			mTextureBaseLevel = 0;
			mTextureMaxLevel = 1000;
			
			// swizzle parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
			mSwizzleR = eSwizzle::RED;
			mSwizzleG = eSwizzle::GREEN;
			mSwizzleB = eSwizzle::BLUE;
			mSwizzleA = eSwizzle::ALPHA;
			
			// filter parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			mMinFilter = eMinFilter::NEAREST;
			mMagFilter = eMagFilter::NEAREST;

			// wrapping modes
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			mWrapS = eWrapping::REPEAT;
			mWrapT = eWrapping::REPEAT;

			auto fmt = mConverter.translate(format);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			
			// set the width/height at a given mip level
			glTexStorage2D(
				GL_TEXTURE_2D, 
				1, // mip level
				fmt.Internal, 
				static_cast<GLsizei>(width),	
				static_cast<GLsizei>(height)
			);

			// submit the data
			glTexSubImage2D(
				GL_TEXTURE_2D,
				0, // current level
				0, // x offset
				0, // y offset
				static_cast<GLsizei>(width),
				static_cast<GLsizei>(height),
				fmt.External,
				fmt.Type,
				(*mData)[0].data()
			);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			unbind();
		}

		Texture2D::Texture2D(const std::string& filepath):
			mHandle(0)
		{
			// first try to load it using gli (.dds, .kmg, .ktx)
			gli::texture tex = gli::load(filepath);
			
			if (tex.empty()) {
				// try stb for single-image formats (.png, .jpg, .bmp etc)
				int imageWidth = 0;
				int imageHeight = 0;
				int numChannels = 0;

				struct FreeHelper {
					void operator()(unsigned char* buffer) {
						free(buffer);
					}
				};

				std::unique_ptr<unsigned char[], FreeHelper> data(
					stbi_load(
						filepath.c_str(), 
						&imageWidth, 
						&imageHeight, 
						&numChannels, 
						0
					)
				);

				if (data) {
					gli::format fmt;

					switch (numChannels) {
					case 1: fmt = gli::FORMAT_R8_UNORM_PACK8; break;
					case 2: fmt = gli::FORMAT_RG8_UNORM_PACK8; break;
					case 3: fmt = gli::FORMAT_RGB8_UNORM_PACK8; break;
					case 4: fmt = gli::FORMAT_RGBA8_UNORM_PACK8; break;
					default:
						gLogError << "Unsupported number of channels found in source file";
						throw std::runtime_error("Unsupported number of channels found");
					}

					mData = std::make_unique<gli::texture2D>(fmt, glm::ivec2(imageWidth, imageHeight));

					memcpy(
						(*mData)[0].data(), 
						data.get(), 
						imageWidth * imageHeight * numChannels
					);
				}
				else {
					gLogError << stbi_failure_reason();
					throw std::runtime_error(stbi_failure_reason());
				}
			}
			else {
				if (tex.target() != gli::TARGET_2D) {
					gLogError << "Not a 2D texture" << tex.target();
					throw std::runtime_error("Incorrect texture type");
				}

				mData = std::make_unique<gli::texture2D>(tex);
			}

			glGenTextures(1, &mHandle);

			if (mHandle == 0)
				throw std::runtime_error("Failed to generate a new texture");

			bind();

			// assume only 1 mip level
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);
			mTextureBaseLevel = 0;
			mTextureMaxLevel = 1000;

			// swizzle parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
			mSwizzleR = eSwizzle::RED;
			mSwizzleG = eSwizzle::GREEN;
			mSwizzleB = eSwizzle::BLUE;
			mSwizzleA = eSwizzle::ALPHA;

			// filter parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			mMinFilter = eMinFilter::NEAREST;
			mMagFilter = eMagFilter::NEAREST;

			// wrapping modes
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			mWrapS = eWrapping::REPEAT;
			mWrapT = eWrapping::REPEAT;

			auto fmt = mConverter.translate(mData->format());

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// set the width/height at a given mip level
			int width = mData->dimensions().x;
			int height = mData->dimensions().y;

			glTexStorage2D(
				GL_TEXTURE_2D,
				1, // mip level
				fmt.Internal,
				static_cast<GLsizei>(width),
				static_cast<GLsizei>(height)
			);

			// submit the data
			glTexSubImage2D(
				GL_TEXTURE_2D,
				0, // current level
				0, // x offset
				0, // y offset
				static_cast<GLsizei>(width),
				static_cast<GLsizei>(height),
				fmt.External,
				fmt.Type,
				(*mData)[0].data()
			);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			unbind();
		}

		Texture2D::~Texture2D() {
			if (mHandle)
				glDeleteTextures(1, &mHandle);
		}

		GLuint Texture2D::getHandle() const {
			return mHandle;
		}
		
		eTextureFormat Texture2D::getFormat() const {
			return mData->format();
		}

		eTextureFormatType Texture2D::getFormatType() const {
			switch (getFormat()) {
				case gli::FORMAT_RG4_UNORM_PACK8:
				case gli::FORMAT_RGBA4_UNORM_PACK16:
				case gli::FORMAT_BGRA4_UNORM_PACK16:
				case gli::FORMAT_R5G6B5_UNORM_PACK16:
				case gli::FORMAT_B5G6R5_UNORM_PACK16:
				case gli::FORMAT_RGB5A1_UNORM_PACK16:
				case gli::FORMAT_BGR5A1_UNORM_PACK16:
				case gli::FORMAT_A1RGB5_UNORM_PACK16:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_R8_UNORM_PACK8:
				case gli::FORMAT_R8_SNORM_PACK8:
				case gli::FORMAT_R8_USCALED_PACK8:
				case gli::FORMAT_R8_SSCALED_PACK8:
				case gli::FORMAT_R8_UINT_PACK8:
				case gli::FORMAT_R8_SINT_PACK8:
				case gli::FORMAT_R8_SRGB_PACK8:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RG8_UNORM_PACK8:
				case gli::FORMAT_RG8_SNORM_PACK8:
				case gli::FORMAT_RG8_USCALED_PACK8:
				case gli::FORMAT_RG8_SSCALED_PACK8:
				case gli::FORMAT_RG8_UINT_PACK8:
				case gli::FORMAT_RG8_SINT_PACK8:
				case gli::FORMAT_RG8_SRGB_PACK8:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGB8_UNORM_PACK8:
				case gli::FORMAT_RGB8_SNORM_PACK8:
				case gli::FORMAT_RGB8_USCALED_PACK8:
				case gli::FORMAT_RGB8_SSCALED_PACK8:
				case gli::FORMAT_RGB8_UINT_PACK8:
				case gli::FORMAT_RGB8_SINT_PACK8:
				case gli::FORMAT_RGB8_SRGB_PACK8:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_BGR8_UNORM_PACK8:
				case gli::FORMAT_BGR8_SNORM_PACK8:
				case gli::FORMAT_BGR8_USCALED_PACK8:
				case gli::FORMAT_BGR8_SSCALED_PACK8:
				case gli::FORMAT_BGR8_UINT_PACK8:
				case gli::FORMAT_BGR8_SINT_PACK8:
				case gli::FORMAT_BGR8_SRGB_PACK8:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGBA8_UNORM_PACK8:
				case gli::FORMAT_RGBA8_SNORM_PACK8:
				case gli::FORMAT_RGBA8_USCALED_PACK8:
				case gli::FORMAT_RGBA8_SSCALED_PACK8:
				case gli::FORMAT_RGBA8_UINT_PACK8:
				case gli::FORMAT_RGBA8_SINT_PACK8:
				case gli::FORMAT_RGBA8_SRGB_PACK8:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_BGRA8_UNORM_PACK8:
				case gli::FORMAT_BGRA8_SNORM_PACK8:
				case gli::FORMAT_BGRA8_USCALED_PACK8:
				case gli::FORMAT_BGRA8_SSCALED_PACK8:
				case gli::FORMAT_BGRA8_UINT_PACK8:
				case gli::FORMAT_BGRA8_SINT_PACK8:
				case gli::FORMAT_BGRA8_SRGB_PACK8:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGBA8_UNORM_PACK32:
				case gli::FORMAT_RGBA8_SNORM_PACK32:
				case gli::FORMAT_RGBA8_USCALED_PACK32:
				case gli::FORMAT_RGBA8_SSCALED_PACK32:
				case gli::FORMAT_RGBA8_UINT_PACK32:
				case gli::FORMAT_RGBA8_SINT_PACK32:
				case gli::FORMAT_RGBA8_SRGB_PACK32:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGB10A2_UNORM_PACK32:
				case gli::FORMAT_RGB10A2_SNORM_PACK32:
				case gli::FORMAT_RGB10A2_USCALED_PACK32:
				case gli::FORMAT_RGB10A2_SSCALED_PACK32:
				case gli::FORMAT_RGB10A2_UINT_PACK32:
				case gli::FORMAT_RGB10A2_SINT_PACK32:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_BGR10A2_UNORM_PACK32:
				case gli::FORMAT_BGR10A2_SNORM_PACK32:
				case gli::FORMAT_BGR10A2_USCALED_PACK32:
				case gli::FORMAT_BGR10A2_SSCALED_PACK32:
				case gli::FORMAT_BGR10A2_UINT_PACK32:
				case gli::FORMAT_BGR10A2_SINT_PACK32:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_R16_UNORM_PACK16:
				case gli::FORMAT_R16_SNORM_PACK16:
				case gli::FORMAT_R16_USCALED_PACK16:
				case gli::FORMAT_R16_SSCALED_PACK16:
				case gli::FORMAT_R16_UINT_PACK16:
				case gli::FORMAT_R16_SINT_PACK16:
				case gli::FORMAT_R16_SFLOAT_PACK16:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RG16_UNORM_PACK16:
				case gli::FORMAT_RG16_SNORM_PACK16:
				case gli::FORMAT_RG16_USCALED_PACK16:
				case gli::FORMAT_RG16_SSCALED_PACK16:
				case gli::FORMAT_RG16_UINT_PACK16:
				case gli::FORMAT_RG16_SINT_PACK16:
				case gli::FORMAT_RG16_SFLOAT_PACK16:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGB16_UNORM_PACK16:
				case gli::FORMAT_RGB16_SNORM_PACK16:
				case gli::FORMAT_RGB16_USCALED_PACK16:
				case gli::FORMAT_RGB16_SSCALED_PACK16:
				case gli::FORMAT_RGB16_UINT_PACK16:
				case gli::FORMAT_RGB16_SINT_PACK16:
				case gli::FORMAT_RGB16_SFLOAT_PACK16:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGBA16_UNORM_PACK16:
				case gli::FORMAT_RGBA16_SNORM_PACK16:
				case gli::FORMAT_RGBA16_USCALED_PACK16:
				case gli::FORMAT_RGBA16_SSCALED_PACK16:
				case gli::FORMAT_RGBA16_UINT_PACK16:
				case gli::FORMAT_RGBA16_SINT_PACK16:
				case gli::FORMAT_RGBA16_SFLOAT_PACK16:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_R32_UINT_PACK32:
				case gli::FORMAT_R32_SINT_PACK32:
				case gli::FORMAT_R32_SFLOAT_PACK32:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RG32_UINT_PACK32:
				case gli::FORMAT_RG32_SINT_PACK32:
				case gli::FORMAT_RG32_SFLOAT_PACK32:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGB32_UINT_PACK32:
				case gli::FORMAT_RGB32_SINT_PACK32:
				case gli::FORMAT_RGB32_SFLOAT_PACK32:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGBA32_UINT_PACK32:
				case gli::FORMAT_RGBA32_SINT_PACK32:
				case gli::FORMAT_RGBA32_SFLOAT_PACK32:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_R64_UINT_PACK64:
				case gli::FORMAT_R64_SINT_PACK64:
				case gli::FORMAT_R64_SFLOAT_PACK64:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RG64_UINT_PACK64:
				case gli::FORMAT_RG64_SINT_PACK64:
				case gli::FORMAT_RG64_SFLOAT_PACK64:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGB64_UINT_PACK64:
				case gli::FORMAT_RGB64_SINT_PACK64:
				case gli::FORMAT_RGB64_SFLOAT_PACK64:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGBA64_UINT_PACK64:
				case gli::FORMAT_RGBA64_SINT_PACK64:
				case gli::FORMAT_RGBA64_SFLOAT_PACK64:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RG11B10_UFLOAT_PACK32:
				case gli::FORMAT_RGB9E5_UFLOAT_PACK32:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_D16_UNORM_PACK16:
				case gli::FORMAT_D24_UNORM_PACK32:
				case gli::FORMAT_D32_SFLOAT_PACK32:
					return eTextureFormatType::DEPTH;

				case gli::FORMAT_S8_UINT_PACK8:
					return eTextureFormatType::STENCIL;

				case gli::FORMAT_D16_UNORM_S8_UINT_PACK32:
				case gli::FORMAT_D24_UNORM_S8_UINT_PACK32:
				case gli::FORMAT_D32_SFLOAT_S8_UINT_PACK64:
					return eTextureFormatType::DEPTH_STENCIL;

				case gli::FORMAT_RGB_DXT1_UNORM_BLOCK8:
				case gli::FORMAT_RGB_DXT1_SRGB_BLOCK8:
				case gli::FORMAT_RGBA_DXT1_UNORM_BLOCK8:
				case gli::FORMAT_RGBA_DXT1_SRGB_BLOCK8:
				case gli::FORMAT_RGBA_DXT3_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_DXT3_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_DXT5_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_DXT5_SRGB_BLOCK16:
				case gli::FORMAT_R_ATI1N_UNORM_BLOCK8:
				case gli::FORMAT_R_ATI1N_SNORM_BLOCK8:
				case gli::FORMAT_RG_ATI2N_UNORM_BLOCK16:
				case gli::FORMAT_RG_ATI2N_SNORM_BLOCK16:
				case gli::FORMAT_RGB_BP_UFLOAT_BLOCK16:
				case gli::FORMAT_RGB_BP_SFLOAT_BLOCK16:
				case gli::FORMAT_RGBA_BP_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_BP_SRGB_BLOCK16:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGB_ETC2_UNORM_BLOCK8:
				case gli::FORMAT_RGB_ETC2_SRGB_BLOCK8:
				case gli::FORMAT_RGBA_ETC2_UNORM_BLOCK8:
				case gli::FORMAT_RGBA_ETC2_SRGB_BLOCK8:
				case gli::FORMAT_RGBA_ETC2_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ETC2_SRGB_BLOCK16:
				case gli::FORMAT_R_EAC_UNORM_BLOCK8:
				case gli::FORMAT_R_EAC_SNORM_BLOCK8:
				case gli::FORMAT_RG_EAC_UNORM_BLOCK16:
				case gli::FORMAT_RG_EAC_SNORM_BLOCK16:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGBA_ASTC_4X4_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_4X4_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_5X4_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_5X4_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_5X5_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_5X5_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_6X5_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_6X5_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_6X6_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_6X6_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_8X5_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_8X5_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_8X6_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_8X6_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_8X8_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_8X8_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_10X5_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_10X5_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_10X6_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_10X6_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_10X8_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_10X8_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_10X10_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_10X10_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_12X10_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_12X10_SRGB_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_12X12_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ASTC_12X12_SRGB_BLOCK16:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGB_PVRTC1_8X8_UNORM_BLOCK32:
				case gli::FORMAT_RGB_PVRTC1_8X8_SRGB_BLOCK32:
				case gli::FORMAT_RGB_PVRTC1_16X8_UNORM_BLOCK32:
				case gli::FORMAT_RGB_PVRTC1_16X8_SRGB_BLOCK32:
				case gli::FORMAT_RGBA_PVRTC1_8X8_UNORM_BLOCK32:
				case gli::FORMAT_RGBA_PVRTC1_8X8_SRGB_BLOCK32:
				case gli::FORMAT_RGBA_PVRTC1_16X8_UNORM_BLOCK32:
				case gli::FORMAT_RGBA_PVRTC1_16X8_SRGB_BLOCK32:
				case gli::FORMAT_RGBA_PVRTC2_4X4_UNORM_BLOCK8:
				case gli::FORMAT_RGBA_PVRTC2_4X4_SRGB_BLOCK8:
				case gli::FORMAT_RGBA_PVRTC2_8X4_UNORM_BLOCK8:
				case gli::FORMAT_RGBA_PVRTC2_8X4_SRGB_BLOCK8:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RGB_ETC_UNORM_BLOCK8:
				case gli::FORMAT_RGB_ATC_UNORM_BLOCK8:
				case gli::FORMAT_RGBA_ATCA_UNORM_BLOCK16:
				case gli::FORMAT_RGBA_ATCI_UNORM_BLOCK16:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_L8_UNORM_PACK8:
				case gli::FORMAT_A8_UNORM_PACK8:
				case gli::FORMAT_LA8_UNORM_PACK8:
				case gli::FORMAT_L16_UNORM_PACK16:
				case gli::FORMAT_A16_UNORM_PACK16:
				case gli::FORMAT_LA16_UNORM_PACK16:
					return eTextureFormatType::COLOR; // ... welll luminance/alpha buffers aren't really color

				case gli::FORMAT_BGR8_UNORM_PACK32:
				case gli::FORMAT_BGR8_SRGB_PACK32:
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RG3B2_UNORM_PACK8:
					return eTextureFormatType::COLOR;

				default:
					return eTextureFormatType::UNKNOWN;
			};
		}

		void Texture2D::setWrapS(eWrapping mode) {
			bind();
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(mode));
			mWrapS = mode;

			unbind();
		}

		void Texture2D::setWrapT(eWrapping mode) {
			bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(mode));
			mWrapT = mode;

			unbind();
		}
		
		eWrapping Texture2D::getWrapS() const {
			return mWrapS;
		}

		eWrapping Texture2D::getWrapT() const {
			return mWrapT;
		}
		
		void Texture2D::setFilter(eMinFilter minification) {
			bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(minification));
			mMinFilter = minification;

			unbind();
		}

		void Texture2D::setFilter(eMagFilter magnification) {
			bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(magnification));
			mMagFilter = magnification;

			unbind();
		}

		eMinFilter Texture2D::getMinFilter() const {
			return mMinFilter;
		}

		eMagFilter Texture2D::getMagFilter() const {
			return mMagFilter;
		}
		
		void Texture2D::setSwizzleR(eSwizzle mode) {
			bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, static_cast<GLenum>(mode));
			mSwizzleR = mode;

			unbind();
		}

		void Texture2D::setSwizzleG(eSwizzle mode) {
			bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, static_cast<GLenum>(mode));
			mSwizzleG = mode;

			unbind();
		}

		void Texture2D::setSwizzleB(eSwizzle mode) {
			bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, static_cast<GLenum>(mode));
			mSwizzleB = mode;

			unbind();
		}

		void Texture2D::setSwizzleA(eSwizzle mode) {
			bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, static_cast<GLenum>(mode));
			mSwizzleA = mode;

			unbind();
		}

		void Texture2D::setSwizzleRGBA(eSwizzle mode) {
			bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, static_cast<GLenum>(mode));
			mSwizzleR = mode;
			mSwizzleG = mode;
			mSwizzleB = mode;
			mSwizzleA = mode;

			unbind();
		}

		eSwizzle Texture2D::getSwizzleR() const {
			return mSwizzleR;
		}

		eSwizzle Texture2D::getSwizzleG() const {
			return mSwizzleG;
		}

		eSwizzle Texture2D::getSwizzleB() const {
			return mSwizzleB;
		}

		eSwizzle Texture2D::getSwizzleA() const {
			return mSwizzleA;
		}

		void Texture2D::bind() {
			glBindTexture(GL_TEXTURE_2D, mHandle);
		}

		void Texture2D::unbind() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture2D::bind(int textureUnit) {
			//glBindTextureUnit(textureUnit, mHandle);
			glActiveTexture(asTextureUnit(textureUnit));
			glBindTexture(GL_TEXTURE_2D, mHandle);
		}

		void Texture2D::unbind(int textureUnit) {
			//glBindTextureUnit(textureUnit, 0);
			glActiveTexture(asTextureUnit(textureUnit));
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}