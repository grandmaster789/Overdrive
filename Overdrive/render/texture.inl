#pragma once

#include "texture.h"

namespace overdrive {
	namespace render {
		inline std::ostream& operator << (std::ostream& os, const eTextureFormatType& value) {
			switch (value) {
			case eTextureFormatType::COLOR:			os << "COLOR"; break;
			case eTextureFormatType::DEPTH:			os << "DEPTH"; break;
			case eTextureFormatType::STENCIL:		os << "STENCIL"; break;
			case eTextureFormatType::DEPTH_STENCIL: os << "DEPTH_STENCIL"; break;

			default: os << "Unknown"; 
			}

			return os;
		}

		inline std::ostream& operator << (std::ostream& os, const eWrapping& value) {
			switch (value) {
			case eWrapping::BORDER:				os << "BORDER"; break;
			case eWrapping::EDGE:				os << "EDGE"; break;
			case eWrapping::MIRRORED_EDGE:		os << "MIRRORED_EDGE"; break;
			case eWrapping::MIRRORED_REPEAT:	os << "MIRRORED_REPEAT"; break;
			case eWrapping::REPEAT:				os << "REPEAT"; break;

			default: os << "Unknown";
			}

			return os;
		}

		inline std::ostream& operator << (std::ostream& os, const eMinFilter& value) {
			switch (value) {
			case eMinFilter::NEAREST:	os << "NEAREST"; break;
			case eMinFilter::LINEAR:	os << "LINEAR"; break;
			
			case eMinFilter::NEAREST_MIPMAP_NEAREST:	os << "NEAREST_MIPMAP_NEAREST"; break;
			case eMinFilter::NEAREST_MIPMAP_LINEAR:		os << "NEAREST_MIPMAP_LINEAR"; break;
			case eMinFilter::LINEAR_MIPMAP_NEAREST:		os << "LINEAR_MIPMAP_NEAREST"; break;
			case eMinFilter::LINEAR_MIPMAP_LINEAR:		os << "LINEAR_MIPMAP_LINEAR"; break;

			default: os << "Unknown";
			}

			return os;
		}

		inline std::ostream& operator << (std::ostream& os, const eMagFilter& value) {
			switch (value) {
			case eMagFilter::NEAREST:	os << "NEAREST"; break;
			case eMagFilter::LINEAR:	os << "LINEAR"; break;

			default: os << "Unknown";
			}

			return os;
		}

		inline std::ostream& operator << (std::ostream& os, const eSwizzle& value) {
			switch (value) {
			case eSwizzle::RED:		os << "RED"; break;
			case eSwizzle::GREEN:	os << "GREEN"; break;
			case eSwizzle::BLUE:	os << "BLUE"; break;
			case eSwizzle::ALPHA:	os << "ALPHA"; break;
			case eSwizzle::ZERO:	os << "ZERO"; break;
			case eSwizzle::ONE:		os << "ONE"; break;

			default: os << "Unknown";
			}

			return os;
		}
	}
}
