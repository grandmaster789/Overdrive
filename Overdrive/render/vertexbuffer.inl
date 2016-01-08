#pragma once

#include "vertexbuffer.h"

namespace overdrive {
	namespace render {
		template <typename T>
		VertexBuffer<T>::VertexBuffer(
			size_t numElements,
			eBufferUsage usage
		):
			Buffer(eBufferTarget::ARRAY, numElements, usage)
		{
		}

		template <typename T>
		typename VertexBuffer<T>::Data VertexBuffer<T>::map() {
			return map(eBufferAccess::WRITE, eBufferAccess::INVALIDATE_BUFFER);
		}

		template <typename T>
		void VertexBuffer<T>::bind() {

		}

		template <typename T>
		void VertexBuffer<T>::unbind() {
		}

		template <typename T>
		GLuint VertexBuffer<T>::getNumAttributes() const {
			return boost::fusion::result_of::size<T>::type::value;
		}
	}
}
