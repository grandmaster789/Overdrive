#pragma once

#include "indexbuffer.h"
#include "vertexbuffer.h"
#include "vertexarray.h"

namespace overdrive {
	namespace render {
		namespace shape {
			// [NOTE] this assembles the cube in the constructor, so it needs to have a valid, active openGL context
			class Cube {
			public:
				using VertexFormat = attributes::PositionNormalTexCoord;
				using VertexBuffer = VertexBuffer<VertexFormat>;
				using IndexFormat = GLuint;
				using IndexBuffer = IndexBuffer<IndexFormat>;

				Cube(float size = 1.0f);

				void draw();

			private:
				VertexBuffer mVertexBuffer;
				IndexBuffer mIndexBuffer;
				VertexArray mVAO;
			};
		}
	}
}
