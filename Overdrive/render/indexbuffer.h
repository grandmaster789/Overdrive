#pragma once

#include "buffer.h"

namespace overdrive {
	namespace render {
		// [NOTE] For a valid index buffer the type T should be one of [GLubyte, GLushort, GLuint]
		//		  (see http://docs.gl/gl4/glDrawElements)
		template <typename T>
		class IndexBuffer:
			public Buffer<T>
		{
		public:
			typedef Buffer<T> Base;

			IndexBuffer(size_t numElements, eBufferUsage usage = eBufferUsage::STATIC_DRAW);

			Data map(); // ~> WRITE | INVALIDATE_BUFFER
		};
	}
}

#include "indexbuffer.inl"