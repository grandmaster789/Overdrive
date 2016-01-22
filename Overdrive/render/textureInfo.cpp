#include "stdafx.h"
#include "texture.h"

namespace overdrive {
	namespace render {
		GLenum getBaseFormat(eTextureFormat fmt) {
			switch (fmt) {
			case eTextureFormat::RED:				return GL_RED;
			case eTextureFormat::RG:				return GL_RG;
			case eTextureFormat::RGB:				return GL_RGB;
			case eTextureFormat::RGBA:				return GL_RGBA;
			case eTextureFormat::DEPTH_COMPONENT:	return GL_DEPTH_COMPONENT;
			case eTextureFormat::STENCIL:			return GL_STENCIL;
			case eTextureFormat::DEPTH_STENCIL:		return GL_DEPTH_STENCIL;

			case eTextureFormat::R8:				return GL_RED;
			case eTextureFormat::R16:				return GL_RED;
			case eTextureFormat::RG8:				return GL_RG;
			case eTextureFormat::RG16:				return GL_RG;
			case eTextureFormat::R3_G3_B2:			return GL_RGB;
			case eTextureFormat::RGB4:				return GL_RGB;
			case eTextureFormat::RGB5:				return GL_RGB;
			case eTextureFormat::RGB8:				return GL_RGB;
			case eTextureFormat::RGB10:				return GL_RGB;
			case eTextureFormat::RGB12:				return GL_RGB;
			case eTextureFormat::RGBA2:				return GL_RGBA;
			case eTextureFormat::RGBA4:				return GL_RGBA;
			case eTextureFormat::RGB5_A1:			return GL_RGBA;
			case eTextureFormat::RGBA8:				return GL_RGBA;
			case eTextureFormat::RGBA16:			return GL_RGBA;
			case eTextureFormat::SRGB8:				return GL_RGB;
			case eTextureFormat::SRGB8_ALPHA8:		return GL_RGBA;
			
			case eTextureFormat::R8_SNORM:			return GL_RED;
			case eTextureFormat::R16_SNORM:			return GL_RED;
			case eTextureFormat::RG8_SNORM:			return GL_RG;
			case eTextureFormat::RG16_SNORM:		return GL_RG;
			case eTextureFormat::RGB8_SNORM:		return GL_RGB;
			case eTextureFormat::RGB16_SNORM:		return GL_RGB;
			case eTextureFormat::RGBA8_SNORM:		return GL_RGBA;
			
			case eTextureFormat::R16F:				return GL_RED;
			case eTextureFormat::RG16F:				return GL_RG;
			case eTextureFormat::RGB16F:			return GL_RGB;
			case eTextureFormat::RGBA16F:			return GL_RGBA;
			case eTextureFormat::R32F:				return GL_RED;
			case eTextureFormat::RG32F:				return GL_RG;
			case eTextureFormat::RGB32F:			return GL_RGB;
			case eTextureFormat::RGBA32F:			return GL_RGBA;
			case eTextureFormat::R11F_G11F_B10F:	return GL_RGB;
			
			case eTextureFormat::RGB9_E5:			return GL_RGB;
			
			case eTextureFormat::R8I:				return GL_RED;
			case eTextureFormat::R16I:				return GL_RED;
			case eTextureFormat::R32I:				return GL_RED;
			case eTextureFormat::RG16I:				return GL_RG;
			case eTextureFormat::RG32I:				return GL_RG;
			case eTextureFormat::RGB8I:				return GL_RGB;
			case eTextureFormat::RGB16I:			return GL_RGB;
			case eTextureFormat::RGB32I:			return GL_RGB;
			case eTextureFormat::RGBA32I:			return GL_RGBA;
			
			case eTextureFormat::R8UI:				return GL_RED;
			case eTextureFormat::R16UI:				return GL_RED;
			case eTextureFormat::R32UI:				return GL_RED;
			case eTextureFormat::RG16UI:			return GL_RG;
			case eTextureFormat::RG32UI:			return GL_RG;
			case eTextureFormat::RGB8UI:			return GL_RGB;
			case eTextureFormat::RGB16UI:			return GL_RGB;
			case eTextureFormat::RGB32UI:			return GL_RGB;
			case eTextureFormat::RGBA32UI:			return GL_RGBA;

			case eTextureFormat::DEPTH_COMPONENT16:		return GL_DEPTH_COMPONENT;
			case eTextureFormat::DEPTH_COMPONENT24:		return GL_DEPTH_COMPONENT;
			case eTextureFormat::DEPTH_COMPONENT32:		return GL_DEPTH_COMPONENT;
			case eTextureFormat::DEPTH_COMPONENT32F:	return GL_DEPTH_COMPONENT;
			case eTextureFormat::DEPTH24_STENCIL8:		return GL_DEPTH_STENCIL;
			case eTextureFormat::DEPTH32F_STENCIL8:		return GL_DEPTH_STENCIL;
			case eTextureFormat::STENCIL_INDEX1:		return GL_STENCIL;
			case eTextureFormat::STENCIL_INDEX4:		return GL_STENCIL;
			case eTextureFormat::STENCIL_INDEX8:		return GL_STENCIL;
			case eTextureFormat::STENCIL_INDEX16:		return GL_STENCIL;
			
			case eTextureFormat::COMPRESSED_RED:						return GL_RED;
			case eTextureFormat::COMPRESSED_RG:							return GL_RG;
			case eTextureFormat::COMPRESSED_RGB:						return GL_RGB;
			case eTextureFormat::COMPRESSED_RGBA:						return GL_RGBA;
			case eTextureFormat::COMPRESSED_SRGB:						return GL_RGB;
			case eTextureFormat::COMPRESSED_SRGB_ALPHA:					return GL_RGB;
			case eTextureFormat::COMPRESSED_RED_RGTC1:					return GL_RED;
			case eTextureFormat::COMPRESSED_SIGNED_RED_RGTC1:			return GL_RED;
			case eTextureFormat::COMPRESSED_RG_RGTC2:					return GL_RG;
			case eTextureFormat::COMPRESSED_SIGNED_RG_RGTC2:			return GL_RG;
			case eTextureFormat::COMPRESSED_RGBA_BPTC_UNORM:			return GL_RGBA;
			case eTextureFormat::COMPRESSED_SRGBA_ALPHA_BPTC_UNORM:		return GL_RGBA;
			case eTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT:		return GL_RGB;
			case eTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:	return GL_RGB;

			default:
				throw std::runtime_error("Unsupported texture format");
			}
		}

		// [NOTE] not entirely sure I've got this right
		ePixelFormat getDefaultPixelFormat(eTextureFormat fmt) {
			switch (fmt) {
			case eTextureFormat::RED:				return ePixelFormat::RED;
			case eTextureFormat::RG:				return ePixelFormat::RG;
			case eTextureFormat::RGB:				return ePixelFormat::RGB;
			case eTextureFormat::RGBA:				return ePixelFormat::RGBA;
			case eTextureFormat::DEPTH_COMPONENT:	return ePixelFormat::DEPTH_COMPONENT;
			case eTextureFormat::DEPTH_STENCIL:		return ePixelFormat::DEPTH_STENCIL;

			case eTextureFormat::R8:				return ePixelFormat::RED;
			case eTextureFormat::R16:				return ePixelFormat::RED;
			case eTextureFormat::RG8:				return ePixelFormat::RG;
			case eTextureFormat::RG16:				return ePixelFormat::RG;
			case eTextureFormat::R3_G3_B2:			return ePixelFormat::RGB;
			case eTextureFormat::RGB4:				return ePixelFormat::RGB;
			case eTextureFormat::RGB5:				return ePixelFormat::RGB;
			case eTextureFormat::RGB8:				return ePixelFormat::RGB;
			case eTextureFormat::RGB10:				return ePixelFormat::RGB;
			case eTextureFormat::RGB12:				return ePixelFormat::RGB;
			case eTextureFormat::RGBA2:				return ePixelFormat::RGBA;
			case eTextureFormat::RGBA4:				return ePixelFormat::RGBA;
			case eTextureFormat::RGB5_A1:			return ePixelFormat::RGBA;
			case eTextureFormat::RGBA8:				return ePixelFormat::RGBA;
			case eTextureFormat::RGBA16:			return ePixelFormat::RGBA;
			case eTextureFormat::SRGB8:				return ePixelFormat::RGB;
			case eTextureFormat::SRGB8_ALPHA8:		return ePixelFormat::RGBA;
				
			case eTextureFormat::R8_SNORM:			return ePixelFormat::RED;
			case eTextureFormat::R16_SNORM:			return ePixelFormat::RED;
			case eTextureFormat::RG8_SNORM:			return ePixelFormat::RG;
			case eTextureFormat::RG16_SNORM:		return ePixelFormat::RG;
			case eTextureFormat::RGB8_SNORM:		return ePixelFormat::RGB;
			case eTextureFormat::RGB16_SNORM:		return ePixelFormat::RGB;
			case eTextureFormat::RGBA8_SNORM:		return ePixelFormat::RGBA;

			case eTextureFormat::R16F:				return ePixelFormat::RED;
			case eTextureFormat::RG16F:				return ePixelFormat::RG;
			case eTextureFormat::RGB16F:			return ePixelFormat::RGB;
			case eTextureFormat::RGBA16F:			return ePixelFormat::RGBA;
			case eTextureFormat::R32F:				return ePixelFormat::RED;
			case eTextureFormat::RG32F:				return ePixelFormat::RG;
			case eTextureFormat::RGB32F:			return ePixelFormat::RGB;
			case eTextureFormat::RGBA32F:			return ePixelFormat::RGBA;
			case eTextureFormat::R11F_G11F_B10F:	return ePixelFormat::RGB;

			case eTextureFormat::RGB9_E5:			return ePixelFormat::RGB;

			case eTextureFormat::R8I:				return ePixelFormat::RED;
			case eTextureFormat::R16I:				return ePixelFormat::RED;
			case eTextureFormat::R32I:				return ePixelFormat::RED;
			case eTextureFormat::RG16I:				return ePixelFormat::RG;
			case eTextureFormat::RG32I:				return ePixelFormat::RG;
			case eTextureFormat::RGB8I:				return ePixelFormat::RGB;
			case eTextureFormat::RGB16I:			return ePixelFormat::RGB;
			case eTextureFormat::RGB32I:			return ePixelFormat::RGB;
			case eTextureFormat::RGBA32I:			return ePixelFormat::RGBA;

			case eTextureFormat::R8UI:				return ePixelFormat::RED;
			case eTextureFormat::R16UI:				return ePixelFormat::RED;
			case eTextureFormat::R32UI:				return ePixelFormat::RED;
			case eTextureFormat::RG16UI:			return ePixelFormat::RG;
			case eTextureFormat::RG32UI:			return ePixelFormat::RG;
			case eTextureFormat::RGB8UI:			return ePixelFormat::RGB;
			case eTextureFormat::RGB16UI:			return ePixelFormat::RGB;
			case eTextureFormat::RGB32UI:			return ePixelFormat::RGB;
			case eTextureFormat::RGBA32UI:			return ePixelFormat::RGBA;

			case eTextureFormat::DEPTH_COMPONENT16:		return ePixelFormat::DEPTH_COMPONENT;
			case eTextureFormat::DEPTH_COMPONENT24:		return ePixelFormat::DEPTH_COMPONENT;
			case eTextureFormat::DEPTH_COMPONENT32:		return ePixelFormat::DEPTH_COMPONENT;
			case eTextureFormat::DEPTH_COMPONENT32F:	return ePixelFormat::DEPTH_COMPONENT;

			case eTextureFormat::DEPTH24_STENCIL8:		return ePixelFormat::DEPTH_STENCIL;
			case eTextureFormat::DEPTH32F_STENCIL8:		return ePixelFormat::DEPTH_STENCIL;

			case eTextureFormat::COMPRESSED_RED:						return ePixelFormat::RED;
			case eTextureFormat::COMPRESSED_RG:							return ePixelFormat::RG;
			case eTextureFormat::COMPRESSED_RGB:						return ePixelFormat::RGB;
			case eTextureFormat::COMPRESSED_RGBA:						return ePixelFormat::RGBA;
			case eTextureFormat::COMPRESSED_SRGB:						return ePixelFormat::RGB;
			case eTextureFormat::COMPRESSED_SRGB_ALPHA:					return ePixelFormat::RGBA;
			case eTextureFormat::COMPRESSED_RED_RGTC1:					return ePixelFormat::RED;
			case eTextureFormat::COMPRESSED_SIGNED_RED_RGTC1:			return ePixelFormat::RED;
			case eTextureFormat::COMPRESSED_RG_RGTC2:					return ePixelFormat::RG;
			case eTextureFormat::COMPRESSED_SIGNED_RG_RGTC2:			return ePixelFormat::RG;
			case eTextureFormat::COMPRESSED_RGBA_BPTC_UNORM:			return ePixelFormat::RGBA;
			case eTextureFormat::COMPRESSED_SRGBA_ALPHA_BPTC_UNORM:		return ePixelFormat::RGBA;
			case eTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT:		return ePixelFormat::RGB;
			case eTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:	return ePixelFormat::RGB;

			default:
				throw std::runtime_error("Unsupported texture format");
			}
		}

		GLsizei getNumComponents(eTextureFormat fmt) {
			GLenum baseFmt = getBaseFormat(fmt);

			switch (baseFmt) {
			case GL_RED:	return 1;
			case GL_RG:		return 2;
			case GL_RGB:	return 3;
			case GL_RGBA:	return 4;
			
			case GL_DEPTH_COMPONENT: return 1;
			case GL_DEPTH_STENCIL: return 1;

			default:
				throw std::runtime_error("Unsupported texture format");
			}
		}

		std::ostream& operator << (std::ostream& os, const eTextureFormat& value) {
			switch (value) {
			case eTextureFormat::RED:				os << "RED"; break;
			case eTextureFormat::RG:				os << "RG"; break;
			case eTextureFormat::RGB:				os << "RGB"; break;
			case eTextureFormat::RGBA:				os << "RGBA"; break;
			case eTextureFormat::DEPTH_COMPONENT:	os << "Depth component"; break;
			case eTextureFormat::DEPTH_STENCIL:		os << "Depth+Stencil"; break;
													
			case eTextureFormat::R8:				os << "R8"; break;
			case eTextureFormat::R16:				os << "R16"; break;
			case eTextureFormat::RG8:				os << "RG8"; break;
			case eTextureFormat::RG16:				os << "RG16"; break;
			case eTextureFormat::R3_G3_B2:			os << "R3_G3_B2"; break;
			case eTextureFormat::RGB4:				os << "RGB4"; break;
			case eTextureFormat::RGB5:				os << "RGB5"; break;
			case eTextureFormat::RGB8:				os << "RGB8"; break;
			case eTextureFormat::RGB10:				os << "RGB10"; break;
			case eTextureFormat::RGB12:				os << "RGB12"; break;
			case eTextureFormat::RGBA2:				os << "RGBA2"; break;
			case eTextureFormat::RGBA4:				os << "RGBA4"; break;
			case eTextureFormat::RGB5_A1:			os << "RGB5_A1"; break;
			case eTextureFormat::RGBA8:				os << "RGBA8"; break;
			case eTextureFormat::RGBA16:			os << "RGBA16"; break;
			case eTextureFormat::SRGB8:				os << "SRGB8"; break;
			case eTextureFormat::SRGB8_ALPHA8:		os << "SRGB8_ALPHA8"; break;
													
			case eTextureFormat::R8_SNORM:			os << "R8_SNORM"; break;
			case eTextureFormat::R16_SNORM:			os << "R16_SNORM"; break;
			case eTextureFormat::RG8_SNORM:			os << "RG8_SNORM"; break;
			case eTextureFormat::RG16_SNORM:		os << "RG16_SNORM"; break;
			case eTextureFormat::RGB8_SNORM:		os << "RGB8_SNORM"; break;
			case eTextureFormat::RGB16_SNORM:		os << "RGB16_SNORM"; break;
			case eTextureFormat::RGBA8_SNORM:		os << "RGBA8_SNORM"; break;

			case eTextureFormat::R16F:				os << "R16F"; break;
			case eTextureFormat::RG16F:				os << "RG16F"; break;
			case eTextureFormat::RGB16F:			os << "RGB16F"; break;
			case eTextureFormat::RGBA16F:			os << "RGBA16F"; break;
			case eTextureFormat::R32F:				os << "R32F"; break;
			case eTextureFormat::RG32F:				os << "RG32F"; break;
			case eTextureFormat::RGB32F:			os << "RGB32F"; break;
			case eTextureFormat::RGBA32F:			os << "RGBA32F"; break;
			case eTextureFormat::R11F_G11F_B10F:	os << "R11F_G11F_B10F"; break;

			case eTextureFormat::RGB9_E5:			os << "RGB9_E5"; break;

			case eTextureFormat::R8I:				os << "R8I"; break;
			case eTextureFormat::R16I:				os << "R16I"; break;
			case eTextureFormat::R32I:				os << "R32I"; break;
			case eTextureFormat::RG16I:				os << "RG16I"; break;
			case eTextureFormat::RG32I:				os << "RG32I"; break;
			case eTextureFormat::RGB8I:				os << "RGB8I"; break;
			case eTextureFormat::RGB16I:			os << "RGB16I"; break;
			case eTextureFormat::RGB32I:			os << "RGB32I"; break;
			case eTextureFormat::RGBA32I:			os << "RGBA32I"; break;

			case eTextureFormat::R8UI:				os << "R8UI"; break;
			case eTextureFormat::R16UI:				os << "R16UI"; break;
			case eTextureFormat::R32UI:				os << "R32UI"; break;
			case eTextureFormat::RG16UI:			os << "RG16UI"; break;
			case eTextureFormat::RG32UI:			os << "RG32UI"; break;
			case eTextureFormat::RGB8UI:			os << "RGB8UI"; break;
			case eTextureFormat::RGB16UI:			os << "RGB16UI"; break;
			case eTextureFormat::RGB32UI:			os << "RGB32UI"; break;
			case eTextureFormat::RGBA32UI:			os << "RGBA32UI"; break;

			case eTextureFormat::COMPRESSED_RED:						os << "COMPRESSED_RED"; break;
			case eTextureFormat::COMPRESSED_RG:							os << "COMPRESSED_RG"; break;
			case eTextureFormat::COMPRESSED_RGB:						os << "COMPRESSED_RGB"; break;
			case eTextureFormat::COMPRESSED_RGBA:						os << "COMPRESSED_RGBA"; break;
			case eTextureFormat::COMPRESSED_SRGB:						os << "COMPRESSED_SRGB"; break;
			case eTextureFormat::COMPRESSED_SRGB_ALPHA:					os << "COMPRESSED_SRGB_ALPHA"; break;
			case eTextureFormat::COMPRESSED_RED_RGTC1:					os << "COMPRESSED_RED_RGTC1"; break;
			case eTextureFormat::COMPRESSED_SIGNED_RED_RGTC1:			os << "COMPRESSED_SIGNED_RED_RGTC1"; break;
			case eTextureFormat::COMPRESSED_RG_RGTC2:					os << "COMPRESSED_RG_RGTC2"; break;
			case eTextureFormat::COMPRESSED_SIGNED_RG_RGTC2:			os << "COMPRESSED_SIGNED_RG_RGTC2"; break;
			case eTextureFormat::COMPRESSED_RGBA_BPTC_UNORM:			os << "COMPRESSED_RGBA_BPTC_UNORM"; break;
			case eTextureFormat::COMPRESSED_SRGBA_ALPHA_BPTC_UNORM:		os << "COMPRESSED_SRGBA_ALPHA_BPTC_UNORM"; break;
			case eTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT:		os << "COMPRESSED_RGB_BPTC_SIGNED_FLOAT"; break;
			case eTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:	os << "COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT"; break;

			default:
				throw std::runtime_error("Unsupported texture format");
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eTextureBufferFormat& value) {
			switch (value) {
			case eTextureBufferFormat::R8:			os << "R8"; break;
			case eTextureBufferFormat::R16:			os << "R16"; break;
			case eTextureBufferFormat::R16F:		os << "R16F"; break;
			case eTextureBufferFormat::R32F:		os << "R32F"; break;
			case eTextureBufferFormat::R8I:			os << "R8I"; break;
			case eTextureBufferFormat::R16I:		os << "R16I"; break;
			case eTextureBufferFormat::R32I:		os << "R32I"; break;
			case eTextureBufferFormat::R8UI:		os << "R8UI"; break;
			case eTextureBufferFormat::R16UI:		os << "R16UI"; break;
			case eTextureBufferFormat::R32UI:		os << "R32UI"; break;
			case eTextureBufferFormat::RG8:			os << "RG8"; break;
			case eTextureBufferFormat::RG16:		os << "RG16"; break;
			case eTextureBufferFormat::RG16F:		os << "RG16F"; break;
			case eTextureBufferFormat::RG32F:		os << "RG32F"; break;
			case eTextureBufferFormat::RG8I:		os << "RG8I"; break;
			case eTextureBufferFormat::RG16I:		os << "RG16I"; break;
			case eTextureBufferFormat::RG32I:		os << "RG32I"; break;
			case eTextureBufferFormat::RG8UI:		os << "RG8UI"; break;
			case eTextureBufferFormat::RG16UI:		os << "RG16UI"; break;
			case eTextureBufferFormat::RG32UI:		os << "RG32UI"; break;
			case eTextureBufferFormat::RGB32F:		os << "RGB32F"; break;
			case eTextureBufferFormat::RGB32I:		os << "RGB32I"; break;
			case eTextureBufferFormat::RGB32UI:		os << "RGB32UI"; break;
			case eTextureBufferFormat::RGBA8:		os << "RGBA8"; break;
			case eTextureBufferFormat::RGBA16:		os << "RGBA16"; break;
			case eTextureBufferFormat::RGBA16F:		os << "RGBA16F"; break;
			case eTextureBufferFormat::RGBA32F:		os << "RGBA32F"; break;
			case eTextureBufferFormat::RGBA8I:		os << "RGBA8I"; break;
			case eTextureBufferFormat::RGBA16I:		os << "RGBA16I"; break;
			case eTextureBufferFormat::RGBA32I:		os << "RGBA32I"; break;
			case eTextureBufferFormat::RGBA8UI:		os << "RGBA8UI"; break;
			case eTextureBufferFormat::RGBA16UI:	os << "RGBA16UI"; break;
			case eTextureBufferFormat::RGBA32UI:	os << "RGBA32UI"; break;

			default:
				throw std::runtime_error("Unsupported texture buffer format");
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const ePixelFormat& value) {
			switch (value) {
			case ePixelFormat::RED:					os << "RED"; break;
			case ePixelFormat::RG:					os << "RG"; break;
			case ePixelFormat::RGB:					os << "RGB"; break;
			case ePixelFormat::BGR:					os << "BGR"; break;
			case ePixelFormat::RGBA:				os << "RGBA"; break;
			case ePixelFormat::BGRA:				os << "BGRA"; break;
			
			case ePixelFormat::RED_INTEGER:			os << "RED_INTEGER"; break;
			case ePixelFormat::RG_INTEGER:			os << "RG_INTEGER"; break;
			case ePixelFormat::RGB_INTEGER:			os << "RGB_INTEGER"; break;
			case ePixelFormat::BGR_INTEGER:			os << "BGR_INTEGER"; break;
			case ePixelFormat::RGBA_INTEGER:		os << "RGBA_INTEGER"; break;
			case ePixelFormat::BGRA_INTEGER:		os << "BGRA_INTEGER"; break;
			
			case ePixelFormat::STENCIL_INDEX:		os << "STENCIL_INDEX"; break;
			case ePixelFormat::DEPTH_COMPONENT:		os << "DEPTH_COMPONENT"; break;
			case ePixelFormat::DEPTH_STENCIL:		os << "DEPTH_STENCIL"; break;

			default:
				throw std::runtime_error("Unsupported pixel format");
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const ePixelData& value) {
			switch (value) {
			case ePixelData::UNSIGNED_BYTE:		os << "UNSIGNED_BYTE"; break;
			case ePixelData::BYTE:				os << "BYTE"; break;
			case ePixelData::UNSIGNED_SHORT:	os << "UNSIGNED_SHORT"; break;
			case ePixelData::SHORT:				os << "SHORT"; break;
			case ePixelData::UNSIGNED_INT:		os << "UNSIGNED_INT"; break;
			case ePixelData::INT:				os << "INT"; break;
			case ePixelData::FLOAT:				os << "FLOAT"; break;
			
			case ePixelData::UNSIGNED_BYTE_3_3_2:			os << "UNSIGNED_BYTE_3_3_2"; break;
			case ePixelData::UNSIGNED_BYTE_2_3_3_REV:		os << "UNSIGNED_BYTE_2_3_3_REV"; break;
			case ePixelData::UNSIGNED_SHORT_5_6_5:			os << "UNSIGNED_SHORT_5_6_5"; break;
			case ePixelData::UNSIGNED_SHORT_5_6_5_REV:		os << "UNSIGNED_SHORT_5_6_5_REV"; break;
			case ePixelData::UNSIGNED_SHORT_4_4_4_4:		os << "UNSIGNED_SHORT_4_4_4_4"; break;
			case ePixelData::UNSIGNED_SHORT_4_4_4_4_REV:	os << "UNSIGNED_SHORT_4_4_4_4_REV"; break;
			case ePixelData::UNSIGNED_SHORT_5_5_5_1:		os << "UNSIGNED_SHORT_5_5_5_1"; break;
			case ePixelData::UNSIGNED_SHORT_1_5_5_5_REV:	os << "UNSIGNED_SHORT_1_5_5_5_REV"; break;
			case ePixelData::UNSIGNED_INT_8_8_8_8:			os << "UNSIGNED_INT_8_8_8_8"; break;
			case ePixelData::UNSIGNED_INT_8_8_8_8_REV:		os << "UNSIGNED_INT_8_8_8_8_REV"; break;
			case ePixelData::UNSIGNED_INT_10_10_10_2:		os << "UNSIGNED_INT_10_10_10_2"; break;
			case ePixelData::UNSIGNED_INT_2_10_10_10_REV:	os << "UNSIGNED_INT_2_10_10_10_REV"; break;
			
			default:
				throw std::runtime_error("Unsupported pixeldata value");
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eTextureWrapping& value) {
			switch (value) {
			case eTextureWrapping::CLAMP:			os << "CLAMP"; break;
			case eTextureWrapping::BORDER:			os << "BORDER"; break;
			case eTextureWrapping::MIRRORED_REPEAT: os << "MIRRORED_REPEAT"; break;
			case eTextureWrapping::REPEAT:			os << "REPEAT"; break;
			case eTextureWrapping::MIRRORED_CLAMP:	os << "MIRRORED_CLAMP"; break;

			default:
				throw std::runtime_error("Unsupported texture wrapping value");
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eTextureMinFilter& value) {
			switch (value) {
			case eTextureMinFilter::NEAREST: os << "NEAREST"; break;
			case eTextureMinFilter::LINEAR: os << "LINEAR"; break;
			case eTextureMinFilter::NEAREST_MIPMAP_NEAREST: os << "NEAREST_MIPMAP_NEAREST"; break;
			case eTextureMinFilter::NEAREST_MIPMAP_LINEAR: os << "NEAREST_MIPMAP_LINEAR"; break;
			case eTextureMinFilter::LINEAR_MIPMAP_NEAREST: os << "LINEAR_MIPMAP_NEAREST"; break;
			case eTextureMinFilter::LINEAR_MIPMAP_LINEAR: os << "LINEAR_MIPMAP_LINEAR"; break;

			default:
				throw std::runtime_error("Unsupported texture min filter");
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eTextureMagFilter& value) {
			switch (value) {
			case eTextureMagFilter::NEAREST: os << "NEAREST"; break;
			case eTextureMagFilter::LINEAR: os << "LINEAR"; break;

			default:
				throw std::runtime_error("Unsupported texture mag filter");
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eTextureCompareFunction& value) {
			switch (value) {
			case eTextureCompareFunction::LESS:				os << "LESS"; break;
			case eTextureCompareFunction::LESS_EQUAL:		os << "LESS_EQUAL"; break;
			case eTextureCompareFunction::EQUAL:			os << "EQUAL"; break;
			case eTextureCompareFunction::NOT_EQUAL:		os << "NOT_EQUAL"; break;
			case eTextureCompareFunction::GREATER:			os << "GREATER"; break;
			case eTextureCompareFunction::GREATER_EQUAL:	os << "GREATER_EQUAL"; break;
			case eTextureCompareFunction::ALWAYS:			os << "ALWAYS"; break;
			case eTextureCompareFunction::NEVER:			os << "NEVER"; break;

			default:
				throw std::runtime_error("Unsupported texture compare function");
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eTextureCompareMode& value) {
			switch (value) {
			case eTextureCompareMode::REF_TO_TEXTURE:	os << "REF_TO_TEXTURE"; break;
			case eTextureCompareMode::NONE:				os << "NONE"; break;

			default:
				throw std::runtime_error("Unsupported texture compare mode");
			}

			return os;
		}
	}
}