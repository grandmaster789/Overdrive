#ifndef OVERDRIVE_RENDER_VERTEXARRAY_H
#define OVERDRIVE_RENDER_VERTEXARRAY_H

#include "opengl.h"
#include "render/vertexbuffer.h"

namespace overdrive {
	namespace render {
		class VertexArray {
		public:
			VertexArray();
			~VertexArray();

			VertexArray(const VertexArray&) = delete;
			void operator = (const VertexArray&) = delete;

			GLuint getHandle() const;

			// for example, binding 3d position data (vec3f) to location 4 ~> vao.bindAttribute(4, buffer, eElementType::FLOAT, 3)
			void bindAttribute(
				GLuint location,
				const VertexBuffer& buffer,
				eElementType type,
				GLint numElements,
				GLsizei stride = 0,
				const GLvoid* offset = nullptr
			);

			void bindElements(const VertexBuffer& elements);
			void bindTransformFeedback(GLuint index, const VertexBuffer& buffer);

		private:
			GLuint mHandle;
		};
	}
}

#endif
