#include "render/vertexarray.h"

namespace overdrive {
	namespace render {
		VertexArray::VertexArray():
			mHandle(0)
		{
			glGenVertexArrays(1, &mHandle);
		}

		VertexArray::~VertexArray() {
			if (mHandle)
				glDeleteVertexArrays(1, &mHandle);
		}

		GLuint VertexArray::getHandle() const {
			return mHandle;
		}
		
		void VertexArray::bindAttribute(
			GLuint location,
			const VertexBuffer& buffer,
			eElementType type,
			GLint numElements,
			GLsizei stride,
			const GLvoid* offset
		) {
			glBindVertexArray(mHandle);

			glBindBuffer(GL_ARRAY_BUFFER, buffer.getHandle());
			glEnableVertexAttribArray(location);

			glVertexAttribPointer(
				location, 
				numElements, 
				static_cast<GLenum>(type), 
				GL_FALSE, 
				stride,
				offset
			);
		}

		void VertexArray::bindElements(const VertexBuffer& elements) {
			glBindVertexArray(mHandle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements.getHandle());
		}

		void VertexArray::bindTransformFeedback(
			GLuint index, 
			const VertexBuffer& buffer
		) {
			glBindVertexArray(mHandle);
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index, buffer.getHandle());
		}
	}
}