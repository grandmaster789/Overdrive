#pragma once

#include "../opengl.h"
#include <ostream>

namespace overdrive {
	namespace render {
		/*
			Color formats can be stored in one of 3 ways: normalized integers, 
			floating-point, or integral. Both normalized integer and floating-point 
			formats will resolve, in the shader, to a vector of floating-point values, 
			whereas integral formats will resolve to a vector of integers.

			[NOTE] perhaps replace all of this with GLI structures

			Weirdness encountered while encoding the documentation
			- GL_RGB16 doesn't exist (GL_RGB16_SNORM does)
			- GL_RGBA16_SNORM doesn't exist (GL_RGBA16 does)
			- GL_RGB9_E5 is the only format with shared bits, seems like a strange format

			[TODO] perhaps a struct that contains the information from http://docs.gl/gl4/glTexImage2D (table 2) would be good?
			[TODO] there are even more formats listed on https://www.opengl.org/wiki/GLAPI/glTexImage2D (S3TC formats etc)
		*/
		// https://www.opengl.org/wiki/Image_Format
		enum class eTextureFormat: GLint {
			// Base formats
			RED					= GL_RED,
			RG					= GL_RG,
			RGB					= GL_RGB,
			RGBA				= GL_RGBA,
			DEPTH_COMPONENT		= GL_DEPTH_COMPONENT,
			STENCIL				= GL_STENCIL,
			DEPTH_STENCIL		= GL_DEPTH_STENCIL,
			

			// sized formats
			R8					= GL_R8,
			R16					= GL_R16,
			RG8					= GL_RG8,
			RG16				= GL_RG16,
			R3_G3_B2			= GL_R3_G3_B2,
			RGB4				= GL_RGB4,
			RGB5				= GL_RGB5,
			RGB8				= GL_RGB8,
			RGB10				= GL_RGB10,
			RGB12				= GL_RGB12,
			RGBA2				= GL_RGBA2,
			RGBA4				= GL_RGBA4,
			RGB5_A1				= GL_RGB5_A1,
			RGBA8				= GL_RGBA8,
			RGBA16				= GL_RGBA16,
			SRGB8				= GL_SRGB8,
			SRGB8_ALPHA8		= GL_SRGB8_ALPHA8,

			R8_SNORM			= GL_R8_SNORM,
			R16_SNORM			= GL_R16_SNORM,
			RG8_SNORM			= GL_RG8_SNORM,
			RG16_SNORM			= GL_RG16_SNORM,
			RGB8_SNORM			= GL_RGB8_SNORM,
			RGB16_SNORM			= GL_RGB16_SNORM,
			RGBA8_SNORM			= GL_RGBA8_SNORM,

			R16F				= GL_R16F,
			RG16F				= GL_RG16F,
			RGB16F				= GL_RGB16F,
			RGBA16F				= GL_RGBA16F,
			R32F				= GL_R32F,
			RG32F				= GL_RG32F,
			RGB32F				= GL_RGB32F,
			RGBA32F				= GL_RGBA32F,
			R11F_G11F_B10F		= GL_R11F_G11F_B10F,

			RGB9_E5				= GL_RGB9_E5,

			R8I					= GL_R8I,
			R16I				= GL_R16I,
			R32I				= GL_R32I,
			RG16I				= GL_RG16I,
			RG32I				= GL_RG32I,
			RGB8I				= GL_RGB8I,
			RGB16I				= GL_RGB16I,
			RGB32I				= GL_RGB32I,
			RGBA32I				= GL_RGBA32I,

			R8UI				= GL_R8UI,
			R16UI				= GL_R16UI,			
			R32UI				= GL_R32UI,
			RG16UI				= GL_RG16UI,
			RG32UI				= GL_RG32UI,
			RGB8UI				= GL_RGB8UI,
			RGB16UI				= GL_RGB16UI,
			RGB32UI				= GL_RGB32UI,
			RGBA32UI			= GL_RGBA32UI,

			// Sized Depth/stencil formats
			DEPTH_COMPONENT16	= GL_DEPTH_COMPONENT16,
			DEPTH_COMPONENT24	= GL_DEPTH_COMPONENT24,
			DEPTH_COMPONENT32	= GL_DEPTH_COMPONENT32,
			DEPTH_COMPONENT32F	= GL_DEPTH_COMPONENT32F,
			DEPTH24_STENCIL8	= GL_DEPTH24_STENCIL8,
			DEPTH32F_STENCIL8	= GL_DEPTH32F_STENCIL8,
			STENCIL_INDEX1		= GL_STENCIL_INDEX1,
			STENCIL_INDEX4		= GL_STENCIL_INDEX4,
			STENCIL_INDEX8		= GL_STENCIL_INDEX8,
			STENCIL_INDEX16		= GL_STENCIL_INDEX16,

			// compressed formats
			COMPRESSED_RED						= GL_COMPRESSED_RED,
			COMPRESSED_RG						= GL_COMPRESSED_RG,
			COMPRESSED_RGB						= GL_COMPRESSED_RGB,
			COMPRESSED_RGBA						= GL_COMPRESSED_RGBA,
			COMPRESSED_SRGB						= GL_COMPRESSED_SRGB,
			COMPRESSED_SRGB_ALPHA				= GL_COMPRESSED_SRGB_ALPHA,
			COMPRESSED_RED_RGTC1				= GL_COMPRESSED_RED_RGTC1,
			COMPRESSED_SIGNED_RED_RGTC1			= GL_COMPRESSED_SIGNED_RED_RGTC1,
			COMPRESSED_RG_RGTC2					= GL_COMPRESSED_RG_RGTC2,
			COMPRESSED_SIGNED_RG_RGTC2			= GL_COMPRESSED_SIGNED_RG_RGTC2,
			COMPRESSED_RGBA_BPTC_UNORM			= GL_COMPRESSED_RGBA_BPTC_UNORM,
			COMPRESSED_SRGBA_ALPHA_BPTC_UNORM	= GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
			COMPRESSED_RGB_BPTC_SIGNED_FLOAT	= GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
			COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT	= GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
		};

		// This is only used by TextureBuffer
		enum class eTextureBufferFormat: GLenum {
			R8			= GL_R8,
			R16			= GL_R16,
			R16F		= GL_R16F, 
			R32F		= GL_R32F,
			R8I			= GL_R8I,
			R16I		= GL_R16I,
			R32I		= GL_R32I,
			R8UI		= GL_R8UI,
			R16UI		= GL_R16UI,
			R32UI		= GL_R32UI,
			RG8			= GL_RG8,
			RG16		= GL_RG16,
			RG16F		= GL_RG16F,
			RG32F		= GL_RG32F,
			RG8I		= GL_RG8I,
			RG16I		= GL_RG16I,
			RG32I		= GL_RG32I,
			RG8UI		= GL_RG8UI,
			RG16UI		= GL_RG16UI,
			RG32UI		= GL_RG32UI,
			RGB32F		= GL_RGB32F,
			RGB32I		= GL_RGB32I,
			RGB32UI		= GL_RGB32UI,
			RGBA8		= GL_RGBA8,
			RGBA16		= GL_RGBA16,
			RGBA16F		= GL_RGBA16F,
			RGBA32F		= GL_RGBA32F,
			RGBA8I		= GL_RGBA8I,
			RGBA16I		= GL_RGBA16I,
			RGBA32I		= GL_RGBA32I,
			RGBA8UI		= GL_RGBA8UI,
			RGBA16UI	= GL_RGBA16UI,
			RGBA32UI	= GL_RGBA32UI
		};

		enum class ePixelFormat: GLenum {
			RED					= GL_RED, 
			RG					= GL_RG, 
			RGB					= GL_RGB, 
			BGR					= GL_BGR, 
			RGBA				= GL_RGBA, 
			BGRA				= GL_BGRA, 
			
			RED_INTEGER			= GL_RED_INTEGER, 
			RG_INTEGER			= GL_RG_INTEGER, 
			RGB_INTEGER			= GL_RGB_INTEGER, 
			BGR_INTEGER			= GL_BGR_INTEGER, 
			RGBA_INTEGER		= GL_RGBA_INTEGER, 
			BGRA_INTEGER		= GL_BGRA_INTEGER, 
			
			STENCIL_INDEX		= GL_STENCIL_INDEX, 
			DEPTH_COMPONENT		= GL_DEPTH_COMPONENT, 
			DEPTH_STENCIL		= GL_DEPTH_STENCIL
		};

		enum class ePixelData: GLenum {
			UNSIGNED_BYTE				= GL_UNSIGNED_BYTE,
			BYTE						= GL_BYTE,
			UNSIGNED_SHORT				= GL_UNSIGNED_SHORT,
			SHORT						= GL_SHORT,
			UNSIGNED_INT				= GL_UNSIGNED_INT,
			INT							= GL_INT,
			FLOAT						= GL_FLOAT,

			UNSIGNED_BYTE_3_3_2			= GL_UNSIGNED_BYTE_3_3_2,
			UNSIGNED_BYTE_2_3_3_REV		= GL_UNSIGNED_BYTE_2_3_3_REV,
			UNSIGNED_SHORT_5_6_5		= GL_UNSIGNED_SHORT_5_6_5,
			UNSIGNED_SHORT_5_6_5_REV	= GL_UNSIGNED_SHORT_5_6_5_REV,
			UNSIGNED_SHORT_4_4_4_4		= GL_UNSIGNED_SHORT_4_4_4_4,
			UNSIGNED_SHORT_4_4_4_4_REV	= GL_UNSIGNED_SHORT_4_4_4_4_REV,
			UNSIGNED_SHORT_5_5_5_1		= GL_UNSIGNED_SHORT_5_5_5_1,
			UNSIGNED_SHORT_1_5_5_5_REV	= GL_UNSIGNED_SHORT_1_5_5_5_REV,
			UNSIGNED_INT_8_8_8_8		= GL_UNSIGNED_INT_8_8_8_8,
			UNSIGNED_INT_8_8_8_8_REV	= GL_UNSIGNED_INT_8_8_8_8_REV,
			UNSIGNED_INT_10_10_10_2		= GL_UNSIGNED_INT_10_10_10_2,
			UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV
		};

		enum class eTextureWrapping: GLint {
			CLAMP				= GL_CLAMP_TO_EDGE, 
			BORDER				= GL_CLAMP_TO_BORDER, 
			MIRRORED_REPEAT		= GL_MIRRORED_REPEAT, 
			REPEAT				= GL_REPEAT, 
			MIRRORED_CLAMP		= GL_MIRROR_CLAMP_TO_EDGE
		};

		enum class eTextureMinFilter: GLint {
			NEAREST					= GL_NEAREST,
			LINEAR					= GL_LINEAR,
			NEAREST_MIPMAP_NEAREST	= GL_NEAREST_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR	= GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_NEAREST	= GL_LINEAR_MIPMAP_NEAREST,
			LINEAR_MIPMAP_LINEAR	= GL_LINEAR_MIPMAP_LINEAR
		};

		enum class eTextureMagFilter : GLint {
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR
		};

		enum class eTextureCompareFunction: GLenum {
			LESS			= GL_LESS,
			LESS_EQUAL		= GL_LEQUAL,
			EQUAL			= GL_EQUAL,
			NOT_EQUAL		= GL_NOTEQUAL,
			GREATER			= GL_GREATER,
			GREATER_EQUAL	= GL_GEQUAL,
			ALWAYS			= GL_ALWAYS,
			NEVER			= GL_NEVER
		};

		enum class eTextureCompareMode : GLenum {
			REF_TO_TEXTURE	= GL_COMPARE_REF_TO_TEXTURE,
			NONE			= GL_NONE
		};

		GLenum getBaseFormat(eTextureFormat fmt);
		ePixelFormat getDefaultPixelFormat(eTextureFormat fmt);
		GLsizei getNumComponents(eTextureFormat fmt);

		std::ostream& operator << (std::ostream& os, const eTextureFormat& value);
		std::ostream& operator << (std::ostream& os, const eTextureBufferFormat& value);
		std::ostream& operator << (std::ostream& os, const ePixelFormat& value);
		std::ostream& operator << (std::ostream& os, const ePixelData& value);
		std::ostream& operator << (std::ostream& os, const eTextureWrapping& value);
		std::ostream& operator << (std::ostream& os, const eTextureMinFilter& value);
		std::ostream& operator << (std::ostream& os, const eTextureMagFilter& value);
		std::ostream& operator << (std::ostream& os, const eTextureCompareFunction& value);
		std::ostream& operator << (std::ostream& os, const eTextureCompareMode& value);
	}
}