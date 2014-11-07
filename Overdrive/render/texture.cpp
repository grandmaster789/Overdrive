#include "render/texture.h"
#include "util/checkGL.h"

namespace overdrive {
	namespace render {
		Texture::Texture(
			eFormat format,
			eDataType dataType,
			eInternalFormat internalFormat
		):
			mFormat(format),
			mDataType(dataType),
			mInternalFormat(internalFormat)
		{
			glGenTextures(1, &mHandle);
			assert(mHandle != 0);
		}

		Texture::~Texture() {
			glDeleteTextures(1, &mHandle);
		}

		GLuint Texture::getHandle() const {
			return mHandle;		
		}
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::render::Texture::eDataType& datatype) {
	using overdrive::render::Texture;

	os << "Texture data type: ";

	switch (datatype) {
	case Texture::eDataType::BYTE:
		os << "byte";
		break;

	case Texture::eDataType::SHORT:
		os << "short";
		break;

	case Texture::eDataType::INT:
		os << "int";
		break;

	case Texture::eDataType::UNSIGNED_BYTE:
		os << "unsigned byte";
		break;

	case Texture::eDataType::UNSIGNED_SHORT:
		os << "unsigned short";
		break;

	case Texture::eDataType::UNSIGNED_INT:
		os << "unsigned int";
		break;

	case Texture::eDataType::FLOAT:
		os << "float";
		break;

	case Texture::eDataType::DOUBLE:
		os << "double";
		break;

	case Texture::eDataType::UNSIGNED_BYTE_332:
		os << "byte (332)";
		break;

	case Texture::eDataType::UNSIGNED_BYTE_233_REV:
		os << "byte (332 reversed)";
		break;

	case Texture::eDataType::UNSIGNED_SHORT_565:
		os << "unsigned short (565)";
		break;

	case Texture::eDataType::UNSIGNED_SHORT_565_REV:
		os << "unsigned short (565 reversed)";
		break;

	case Texture::eDataType::UNSIGNED_SHORT_4444:
		os << "unsigned short (4444)";
		break;

	case Texture::eDataType::UNSIGNED_SHORT_4444_REV:
		os << "unsigned short (4444 reversed)";
		break;

	case Texture::eDataType::UNSIGNED_SHORT_5551:
		os << "unsigned short (5551)";
		break;

	case Texture::eDataType::UNSIGNED_SHORT_1555_REV:
		os << "unsigned short (1555 reversed)";
		break;

	case Texture::eDataType::UNSIGNED_INT_8888:
		os << "unsigned int (8888)";
		break;

	case Texture::eDataType::UNSIGNED_INT_8888_REV:
		os << "unsigned int (8888 reversed)";
		break;

	case Texture::eDataType::UNSIGNED_INT_1010102:
		os << "unsigned int (1010102)";
		break;

	default:
		os << "unknown";
	}

	return os;
}

std::ostream& operator << (std::ostream& os, const overdrive::render::Texture::eFilter& filter) {
	using overdrive::render::Texture;

	os << "Texture filter: ";

	switch (filter) {
	case Texture::eFilter::NEAREST:
		os << "nearest";
		break;

	case Texture::eFilter::LINEAR:
		os << "linear";
		break;

	case Texture::eFilter::NEAREST_MIPMAP_NEAREST:
		os << "nearest mipmap nearest";
		break;

	case Texture::eFilter::NEAREST_MIPMAP_LINEAR:
		os << "nearest mipmap linear";
		break;

	case Texture::eFilter::LINEAR_MIPMAP_NEAREST:
		os << "linear mipmap nearest";
		break;

	case Texture::eFilter::LINEAR_MIPMAP_LINEAR:
		os << "linear mipmap linear";
		break;

	default:
		os << "unknown";
	}

	return os;
}

std::ostream& operator << (std::ostream& os, const overdrive::render::Texture::eFormat& format) {
	using overdrive::render::Texture;

	os << "Texture format: ";

	switch (format) {
	case Texture::eFormat::RED:
		os << "red";
		break;

	case Texture::eFormat::RGB:
		os << "rgb";
		break;

	case Texture::eFormat::BGR:
		os << "bgr";
		break;

	case Texture::eFormat::RGBA:
		os << "rgba";
		break;

	case Texture::eFormat::BGRA:
		os << "bgra";
		break;

	default:
		os << "unknown";
	}

	return os;
}

std::ostream& operator << (std::ostream& os, const overdrive::render::Texture::eInternalFormat& format) {
	using overdrive::render::Texture;

	os << "Texture internal format: ";

	switch (format) {
	case Texture::eInternalFormat::COMPRESSED_RED:
		os << "compressed red";
		break;

	case Texture::eInternalFormat::COMPRESSED_RG:
		os << "compressed RG";
		break;

	case Texture::eInternalFormat::COMPRESSED_RED_RGTC1:
		os << "compressed RGTC1";
		break;

	case Texture::eInternalFormat::COMPRESSED_RGB:
		os << "compressed RGB";
		break;

	case Texture::eInternalFormat::COMPRESSED_RGBA:
		os << "compressed RGBA";
		break;

	case Texture::eInternalFormat::COMPRESSED_RGRGTC2:
		os << "compressed RGRGTC2";
		break;

	case Texture::eInternalFormat::COMPRESSED_SIGNED_RED_RGTC1:
		os << "compressed signed red RGTC1";
		break;

	case Texture::eInternalFormat::COMPRESSED_SIGNED_RG_RGTC2:
		os << "compressed signed RG RGTC2";
		break;

	case Texture::eInternalFormat::COMPRESSED_SRGB:
		os << "compressed sRGB";
		break;

	case Texture::eInternalFormat::DEPTH_STENCIL:
		os << "depth stencil";
		break;

	case Texture::eInternalFormat::DEPTH24_STENCIL8:
		os << "depth(24) stencil(8)";
		break;

	case Texture::eInternalFormat::DEPTH32F_STENCIL8:
		os << "depth(32f) stencil(8)";
		break;

	case Texture::eInternalFormat::DEPTH_COMPONENT:
		os << "depth component";
		break;

	case Texture::eInternalFormat::DEPTH_COMPONENT16:
		os << "depth component(16)";
		break;

	case Texture::eInternalFormat::DEPTH_COMPONENT24:
		os << "depth component(24)";
		break;

	case Texture::eInternalFormat::DEPTH_COMPONENT32F:
		os << "depth component(32f)";
		break;

	case Texture::eInternalFormat::RED:
		os << "red";
		break;

	case Texture::eInternalFormat::RG:
		os << "RG";
		break;

	case Texture::eInternalFormat::RGB:
		os << "RGB";
		break;

	case Texture::eInternalFormat::RGB4:
		os << "RGB(4)";
		break;

	case Texture::eInternalFormat::RGB5:
		os << "RGB(5)";
		break;

	case Texture::eInternalFormat::RGB5_A1:
		os << "RGB(5) A(1)";
		break;

	case Texture::eInternalFormat::RGBA:
		os << "RGBA";
		break;

	case Texture::eInternalFormat::RGBA2:
		os << "RGBA(2)";
		break;

	case Texture::eInternalFormat::RGBA4:
		os << "RGBA(4)";
		break;

	case Texture::eInternalFormat::R8:
		os << "R(8)";
		break;

	case Texture::eInternalFormat::R8I:
		os << "R(8i)";
		break;

	case Texture::eInternalFormat::R8UI:
		os << "R(8ui)";
		break;

	case Texture::eInternalFormat::RG8:
		os << "RG(8)";
		break;

	case Texture::eInternalFormat::RG8I:
		os << "RG(8i)";
		break;

	case Texture::eInternalFormat::RG8UI:
		os << "RG(8ui)";
		break;

	case Texture::eInternalFormat::RGB8:
		os << "RGB(8)";
		break;

	case Texture::eInternalFormat::RGB8I:
		os << "RGB(8i)";
		break;

	case Texture::eInternalFormat::RGB8UI:
		os << "RGB(8ui)";
		break;

	case Texture::eInternalFormat::RGBA8:
		os << "RGBA(8)";
		break;

	case Texture::eInternalFormat::RGBA8UI:
		os << "RGBA(8ui)";
		break;

	case Texture::eInternalFormat::RGB9E5:
		os << "RGB(9) E(5)";
		break;

	case Texture::eInternalFormat::RGB10:
		os << "RGB(10)";
		break;

	case Texture::eInternalFormat::RGB10A2:
		os << "RGB(10) A(2)";
		break;

	case Texture::eInternalFormat::RGB12:
		os << "RGB(12)";
		break;

	case Texture::eInternalFormat::RGBA12:
		os << "RGBA(12)";
		break;

	case Texture::eInternalFormat::R16F:
		os << "R(16f)";
		break;

	case Texture::eInternalFormat::R16I:
		os << "R(16i)";
		break;

	case Texture::eInternalFormat::R16UI:
		os << "R(16ui)";
		break;
		
	case Texture::eInternalFormat::RG16:
		os << "RG(16)";
		break;

	case Texture::eInternalFormat::RG16F:
		os << "RG(16f)";
		break;

	case Texture::eInternalFormat::RGB16:
		os << "RGB(16)";
		break;

	case Texture::eInternalFormat::RGB16F:
		os << "RGB(16f)";
		break;

	case Texture::eInternalFormat::RGB16I:
		os << "RGB(16i)";
		break;

	case Texture::eInternalFormat::RGB16UI:
		os << "RGB(16ui)";
		break;

	case Texture::eInternalFormat::RGBA16:
		os << "RGBA(16)";
		break;

	case Texture::eInternalFormat::RGBA16F:
		os << "RGBA(16f)";
		break;

	case Texture::eInternalFormat::RGBA16I:
		os << "RGBA(16i)";
		break;

	case Texture::eInternalFormat::RGBA16UI:
		os << "RGBA(16ui)";
		break;

	case Texture::eInternalFormat::R32I:
		os << "R(32i)";
		break;

	case Texture::eInternalFormat::R32F:
		os << "R(32f)";
		break;

	case Texture::eInternalFormat::R32UI:
		os << "R(32ui)";
		break;

	case Texture::eInternalFormat::RG32F:
		os << "RG(32f)";
		break;

	case Texture::eInternalFormat::RG32I:
		os << "RG(32i)";
		break;

	case Texture::eInternalFormat::RG32UI:
		os << "RG(32ui)";
		break;

	case Texture::eInternalFormat::RGB32F:
		os << "RGB(32f)";
		break;

	case Texture::eInternalFormat::RGB32I:
		os << "RGB(32i)";
		break;

	case Texture::eInternalFormat::RGB32UI:
		os << "RGB(32ui)";
		break;

	case Texture::eInternalFormat::RGBA32F:
		os << "RGBA(32f)";
		break;

	case Texture::eInternalFormat::RGBA32I:
		os << "RGBA(32i)";
		break;

	case Texture::eInternalFormat::RGBA32UI:
		os << "RGBA(32ui)";
		break;

	case Texture::eInternalFormat::R3_G3_B2:
		os << "R(3) G(3) B(2)";
		break;

	case Texture::eInternalFormat::R8_SNORM:
		os << "R(8) snorm";
		break;

	case Texture::eInternalFormat::RG8_SNORM:
		os << "RG(8) snorm";
		break;

	case Texture::eInternalFormat::R16_SNORM:
		os << "R(16) snorm";
		break;

	case Texture::eInternalFormat::RG16_SNORM:
		os << "RG(16) snorm";
		break;

	case Texture::eInternalFormat::SRGB8:
		os << "sRGB(8)";
		break;

	case Texture::eInternalFormat::SRGB8A8:
		os << "sRGB(8) A(8)";
		break;

	case Texture::eInternalFormat::SRGBA:
		os << "sRGBA";
		break;

	default:
		os << "unknown";
	}

	return os;
}

std::ostream& operator << (std::ostream& os, const overdrive::render::Texture::eWrapping& wrapping) {
	using overdrive::render::Texture;

	os << "Texture wrapping: ";

	switch (wrapping) {
	case Texture::eWrapping::CLAMP_EDGE:
		os << "clamp to edge";
		break;

	case Texture::eWrapping::CLAMP_BORDER:
		os << "clamp to border";
		break;

	case Texture::eWrapping::REPEAT:
		os << "repeat";
		break;

	case Texture::eWrapping::MIRRORED_REPEAT:
		os << "mirrored repeat";
		break;

	default:
		os << "unknown";
	}

	return os;
}