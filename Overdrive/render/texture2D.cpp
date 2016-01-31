#include "stdafx.h"
#include "texture2D.h"
#include "gltypes.h"
#include "../core/logger.h"

#include <cstdio>

namespace overdrive {
	namespace render {
		Texture2D::Texture2D():
			mHandle(0)
		{
		}

		Texture2D::Texture2D(eTextureFormat fmt, int width, int height):
			mHandle(0),
			mFormat(fmt)
		{
			glGenTextures(1, &mHandle);
			if (mHandle == 0)
				throw std::runtime_error("Unable to allocate a new texture handle");

			glBindTexture(GL_TEXTURE_2D, mHandle);

			auto format = detail::gFormatConverter.translate(fmt);
			
			// submit texture data
			glTexImage2D(
				GL_TEXTURE_2D,
				0,					// base mip level
				format.Internal,
				width,
				height,
				0,					// border (must be 0)
				format.External,
				format.Type,
				nullptr
			);

			// set various flags
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// ~ Swizzle parameters?

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture2D::Texture2D(eTextureFormat fmt, int width, int height, unsigned char* rawData):
			mHandle(0),
			mFormat(fmt)
		{
			glGenTextures(1, &mHandle);
			if (mHandle == 0)
				throw std::runtime_error("Unable to allocate a new texture handle");

			glBindTexture(GL_TEXTURE_2D, mHandle);

			auto format = detail::gFormatConverter.translate(fmt);

			// submit texture data
			glTexImage2D(
				GL_TEXTURE_2D,
				0,					// base mip level
				format.Internal,
				width,
				height,
				0,					// border (must be 0)
				format.External,
				format.Type,
				rawData
			);

			// generate mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);

			// set various flags
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// ~ Swizzle parameters?

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture2D::Texture2D(const gli::texture& tex):
			mHandle(0),
			mFormat(tex.format())
		{
			if (tex.target() != gli::TARGET_2D)
				throw std::runtime_error("GLI texture does not have a 2D target");

			glGenTextures(1, &mHandle);
			if (mHandle == 0)
				throw std::runtime_error("Unabled to allocate a new texture handle");

			glBindTexture(GL_TEXTURE_2D, mHandle);

			auto format = detail::gFormatConverter.translate(tex.format());
			int baseWidth = tex.dimensions().x;
			int baseHeight = tex.dimensions().y;

			glTexStorage2D(
				GL_TEXTURE_2D,
				static_cast<GLint>(tex.levels()),
				format.Internal,
				baseWidth,
				baseHeight
			);

			for (size_t level = 0; level < tex.levels(); ++level) {
				auto levelDimensions = tex.dimensions(level);

				if (gli::is_compressed(tex.format()))
					glCompressedTexSubImage2D(
						GL_TEXTURE_2D,
						static_cast<GLint>(level),
						0, // x offset
						0, // y offset
						levelDimensions.x,
						levelDimensions.y,
						format.Internal,
						static_cast<GLsizei>(tex.size(level)),
						tex.data(0, 0, level) // (layer, face, level)
					);
				else
					glTexSubImage2D(
						GL_TEXTURE_2D,
						static_cast<GLint>(level),
						0, // x offset
						0, // y offset
						levelDimensions.x,
						levelDimensions.y,
						format.Internal,
						format.Type,
						tex.data(0, 0, level) // (layer, face, level)
					);
			}

			// set various flags
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture2D::~Texture2D() {
			if (mHandle)
				glDeleteTextures(1, &mHandle);
		}

		Texture2D::Texture2D(Texture2D&& t):
			mHandle(t.mHandle),
			mFormat(t.mFormat)
		{
			t.mHandle = 0;
		}

		Texture2D& Texture2D::operator = (Texture2D&& t) {
			if (mHandle)
				glDeleteTextures(1, &mHandle);

			mHandle = t.mHandle;
			mFormat = t.mFormat;

			t.mHandle = 0;

			return *this;
		}
		
		GLuint Texture2D::getHandle() const {
			return mHandle;
		}

		eTextureFormat Texture2D::getFormat() const {
			return mFormat;
		}

		eTextureFormatType Texture2D::getFormatType() const {
			switch (mFormat) {
				case gli::FORMAT_UNDEFINED:
					return eTextureFormatType::UNKNOWN;

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
					return eTextureFormatType::UNKNOWN;

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
					return eTextureFormatType::COLOR; // ~ well... luminance/alpha isn't really color

				case gli::FORMAT_BGR8_UNORM_PACK32: 
				case gli::FORMAT_BGR8_SRGB_PACK32: 
					return eTextureFormatType::COLOR;

				case gli::FORMAT_RG3B2_UNORM_PACK8: 
					return eTextureFormatType::COLOR;

			default:
				return eTextureFormatType::UNKNOWN;
			}
		}

		void Texture2D::bind() {
			glBindTexture(GL_TEXTURE_2D, mHandle);
		}

		void Texture2D::unbind() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture2D::bind(int textureUnit) {
			glActiveTexture(asTextureUnit(textureUnit));
			glBindTextureUnit(textureUnit, mHandle);
		}

		void Texture2D::unbind(int textureUnit) {
			glBindTextureUnit(textureUnit, 0);
		}

		Texture2D loadTexture2D(const std::string& filename) {
			struct FCloseHelper {
				void operator()(FILE* handle) {
					std::fclose(handle);
				}
			};

			std::unique_ptr<FILE, FCloseHelper> file(
				std::fopen(filename.c_str(), "rb")
			);

			// figure out how big the file is
			auto begin_of_file = std::ftell(file.get());
			std::fseek(file.get(), 0, SEEK_END);			
			auto end_of_file = std::ftell(file.get());
			std::fseek(file.get(), 0, SEEK_SET);

			std::vector<char> rawData(static_cast<size_t>(end_of_file - begin_of_file));

			std::fread(&rawData[0], 1, rawData.size(), file.get());

			// try to load it as a gli texture
			{
				auto tex = gli::load(&rawData[0], rawData.size());

				if (!tex.empty())
					return Texture2D(tex);
			}

			// try to load it using stbi (with a bit of forceful casting)
			{
				int width = 0;
				int height = 0;
				int numChannels = 0;

				struct FreeHelper {
					void operator()(void* buffer) {
						free(buffer);
					}
				};

				std::unique_ptr<stbi_uc[], FreeHelper> rawTexture(
					stbi_load_from_memory(
						reinterpret_cast<const stbi_uc*>(&rawData[0]), 
						static_cast<int>(rawData.size()), 
						&width, 
						&height, 
						&numChannels, 
						0
					)
				);

				if (rawTexture) {
					// stbi returns a null pointer if it didn't work
					eTextureFormat fmt;

					switch (numChannels) {
					case 1: fmt = gli::FORMAT_R8_UNORM_PACK8; break;
					case 2: fmt = gli::FORMAT_RG8_UNORM_PACK8; break;
					case 3: fmt = gli::FORMAT_RGB8_UNORM_PACK8; break;
					case 4: fmt = gli::FORMAT_RGBA8_UNORM_PACK8; break;

					default:
						throw std::runtime_error("Unsupported number of channels encountered");
					}

					return Texture2D(fmt, width, height, rawTexture.get());
				}
				else
					gLogError << stbi_failure_reason();
					
			}

			// if we reached here, everything is lost -_-
			throw std::runtime_error("Unable to load file");
		}
	}
}
