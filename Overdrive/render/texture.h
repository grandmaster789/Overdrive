#pragma once

#include "../opengl.h"

namespace overdrive {
	namespace render {
		typedef gli::format eTextureFormat;
		typedef gli::target eTextureTarget;
		
		typedef gli::image Image;

		enum class eTextureFormatType {
			COLOR,
			DEPTH,
			STENCIL,
			DEPTH_STENCIL,

			UNKNOWN
		};

		enum class eWrapping: GLenum {
			EDGE				= GL_CLAMP_TO_EDGE,
			BORDER				= GL_CLAMP_TO_BORDER,
			REPEAT				= GL_REPEAT,
			MIRRORED_REPEAT		= GL_MIRRORED_REPEAT,
			MIRRORED_EDGE		= GL_MIRROR_CLAMP_TO_EDGE
		};
		
		enum class eMinFilter: GLenum {
			NEAREST					= GL_NEAREST,
			LINEAR					= GL_LINEAR,
			NEAREST_MIPMAP_NEAREST	= GL_NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST	= GL_LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR	= GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR	= GL_LINEAR_MIPMAP_LINEAR
		};

		enum class eMagFilter: GLenum {
			NEAREST	= GL_NEAREST,
			LINEAR	= GL_LINEAR
		};

		enum class eSwizzle: GLenum {
			RED		= GL_RED, 
			GREEN	= GL_GREEN, 
			BLUE	= GL_BLUE, 
			ALPHA	= GL_ALPHA, 
			ZERO	= GL_ZERO,
			ONE		= GL_ONE
		};

		std::ostream& operator << (std::ostream& os, const eTextureFormatType& value);
		std::ostream& operator << (std::ostream& os, const eWrapping& value);
		std::ostream& operator << (std::ostream& os, const eMinFilter& value);
		std::ostream& operator << (std::ostream& os, const eMagFilter& value);
		std::ostream& operator << (std::ostream& os, const eSwizzle& value);
	}
}

#include "texture.inl"