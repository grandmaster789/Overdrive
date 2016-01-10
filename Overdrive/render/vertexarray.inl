#pragma once

#include "gltypes.h"

namespace overdrive {
	namespace render {
		template <typename T>
		void VertexArray::attach(VertexBuffer<T>& vertexbuffer) {
			bind();
			vertexbuffer.bind();
			unbind();
			vertexbuffer.unbind();

			mVertexBufferSize = static_cast<GLsizei>(vertexbuffer.getSize());
		}

		template <typename T>
		void VertexArray::attach(IndexBuffer<T>& indexbuffer) {
			bind();
			indexBuffer.bind();
			unbind();
			indexBuffer.unbind();

			mIndexBufferSize = indexbuffer.getSize();
			mIndexBufferType = ToValue<T>::value;
		}

		template <typename T>
		void VertexArray::draw(size_t numElements, T* indexbuffer, ePrimitives mode) {
			bind();

			GLenum mode = static_cast<GLenum>(mode);
			GLenum indexBufferType = ToValue<T>::value;

			assert(
				(indexBufferType == GL_UNSIGNED_BYTE) ||
				(indexBufferType == GL_UNSIGNED_SHORT) ||
				(indexBufferType == GL_UNSIGNED_INT)
			);

			glDrawElements(
				mode, 
				numElements, 
				indexBufferType, 
				indexbuffer
			);

			unbind();
		}
	}
}