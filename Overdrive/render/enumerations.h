#ifndef OVERDRIVE_RENDER_ENUMERATIONS_H
#define OVERDRIVE_RENDER_ENUMERATIONS_H

#include "opengl.h"
#include <ostream>

namespace overdrive {
	namespace render {
		// details can be found at https://www.opengl.org/sdk/docs/man/html/glBufferData.xhtml
		enum class eBufferUsage : GLenum {
			STREAM_DRAW = GL_STREAM_DRAW,
			STREAM_READ = GL_STREAM_READ,
			STREAM_COPY = GL_STREAM_COPY,

			STATIC_DRAW = GL_STATIC_DRAW,
			STATIC_READ = GL_STATIC_READ,
			STATIC_COPY = GL_STATIC_COPY,

			DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
			DYNAMIC_READ = GL_DYNAMIC_READ,
			DYNAMIC_COPY = GL_DYNAMIC_COPY
		};
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::render::eBufferUsage& usage);

#endif
