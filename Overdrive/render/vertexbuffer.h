#pragma once

#include "buffer.h"
#include "../opengl.h"
#include "common_vertex_formats.h"

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace overdrive {
	namespace render {
		// [NOTE] the Seq should be a boost fusion sequence (any kind, including an adapted struct)
		//			The internal types should:
		//			- have between 1 and 4 components
		//			- either provide a static const ::components that says how many there are or provide custom traits that do that for you
		//			- components should conform to one of the types in gltypes.h
		//			- the component type should be exposed as ::value_type
		//		  this was made to conform to glm vector types as much as possible (but is still flexible enough for other custom types)
		template <typename Seq>
		class VertexBuffer:
			public Buffer<Seq>
		{
		public:
			typedef Buffer<Seq> Base;

			VertexBuffer(size_t numElements, eBufferUsage usage = eBufferUsage::STATIC_DRAW);
			VertexBuffer(std::initializer_list<Seq> items, eBufferUsage usage = eBufferUsage::STATIC_DRAW);

			Data map(); // ~> WRITE | INVALIDATE_BUFFER

			void bind();
			void unbind();

			GLuint getNumAttributes() const;

		private:
			
		};
	}
}

#include "vertexbuffer.inl"