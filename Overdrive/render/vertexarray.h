#ifndef OVERDRIVE_RENDER_VERTEXARRAY_H
#define OVERDRIVE_RENDER_VERTEXARRAY_H

#include "opengl.h"
#include "render/enumerations.h"
#include "render/vertexbuffer.h"

namespace overdrive {
	namespace render {
		class VertexArray {
		public:
			VertexArray();
			~VertexArray();

			VertexArray(const VertexArray&) = delete;
			VertexArray& operator = (const VertexArray&) = delete;

			GLuint getHandle() const;

			template <typename T, int I>
			void bindAttribute(
				GLuint location,
				const VertexBuffer<T, I>& buffer,
				GLsizei stride = 0,
				const GLvoid* offset = nullptr
			);

			void bindElements(const VertexBuffer<GLuint, 1>& indexBuffer);

		private:
			GLuint mHandle;
		};
	}
}

#include "render/vertexarray.inl"

#endif
