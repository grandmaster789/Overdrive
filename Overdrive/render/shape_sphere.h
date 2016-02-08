#pragma once

#include "indexbuffer.h"
#include "vertexbuffer.h"
#include "vertexarray.h"

namespace overdrive {
	namespace render {
		namespace shape {
			// [NOTE] this assembles the cube in the constructor, so it needs to have a valid, active openGL context
			// [NOTE] this uses the longitude/latitude approach; for some applications a grid distortion would be better though
			class Sphere {
			public:
				using VertexFormat = attributes::PositionNormalTexCoord;
				using VertexBuffer = VertexBuffer<VertexFormat>;
				using IndexFormat = GLuint;
				using IndexBuffer = IndexBuffer<IndexFormat>;

				Sphere(float radius = 1.0f, unsigned int slices = 16, unsigned int stacks = 16);

				void draw();

			private:
				VertexBuffer mVertexBuffer;
				IndexBuffer mIndexBuffer;
				VertexArray mVAO;
			};
		}
	}
}
