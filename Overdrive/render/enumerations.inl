#ifndef OVERDRIVE_RENDER_ENUMERATIONS_INL
#define OVERDRIVE_RENDER_ENUMERATIONS_INL

#include "render/enumerations.h"

namespace overdrive {
	namespace render {
		template <> GLenum GLType<float>::type = GL_FLOAT;
		template <> GLenum GLType<double>::type = GL_DOUBLE;
		template <> GLenum GLType<char>::type = GL_BYTE;
		template <> GLenum GLType<short>::type = GL_SHORT;
		template <> GLenum GLType<int>::type = GL_INT;
		template <> GLenum GLType<unsigned int>::type = GL_UNSIGNED_INT;
		template <> GLenum GLType<unsigned short>::type = GL_UNSIGNED_SHORT;
		template <> GLenum GLType<unsigned char>::type = GL_UNSIGNED_BYTE;
	}
}

#endif
