#pragma once

#include "buffer.h"
#include "../opengl.h"
#include "common_vertex_formats.h"

#include <boost/fusion/include/vector.hpp>

namespace overdrive {
	namespace render {
		// [NOTE] the T should be a boost fusion vector (any kind, including an adapted struct)
		template <typename T>
		class VertexBuffer:
			public Buffer<T>
		{
		public:
			typedef Buffer<T> Base;

			VertexBuffer(size_t numElements, eBufferUsage usage = eBufferUsage::STATIC_DRAW);

			Data map(); // ~> WRITE | INVALIDATE_BUFFER

			void bind();	// [NOTE] this hides the base implementation
			void unbind();	// [NOTE] this hides the base implementation

			GLuint getNumAttributes() const;

		private:
			
		};
	}
}

#include "vertexbuffer.inl"