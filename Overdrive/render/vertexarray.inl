#ifndef OVERDRIVE_RENDER_VERTEXARRAY_INL
#define OVERDRIVE_RENDER_VERTEXARRAY_INL

#include "render/vertexarray.h"

namespace overdrive {
	namespace render {
		template <typename T, int I>
		void VertexArray::bindAttribute(
			GLuint location,
			const VertexBuffer<T, I>& buffer,
			GLsizei stride,
			const GLvoid* offset
		) {
			glBindVertexArray(mHandle);

			glBindBuffer(GL_ARRAY_BUFFER, buffer.getHandle());
			glVertexAttribPointer(
				location,
				I,
				GLType<T>::type,
				GL_FALSE, // never normalized
				stride,
				offset
			);
			glEnableVertexAttribArray(location);
		}
	}
}

#endif
