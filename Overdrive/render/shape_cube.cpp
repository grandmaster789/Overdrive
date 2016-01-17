#include "stdafx.h"
#include "shape_cube.h"

namespace overdrive {
	namespace render {
		namespace shape {
			using attributes::PositionNormalTexCoord;
			using glm::vec3;
			using glm::vec2;

			Cube::Cube(float size):
				mVertexBuffer({
					// Front
					PositionNormalTexCoord{ vec3(-0.5f * size, -0.5f * size,  0.5f * size), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f) },
					PositionNormalTexCoord{ vec3( 0.5f * size, -0.5f * size,  0.5f * size), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f) },
					PositionNormalTexCoord{ vec3( 0.5f * size,  0.5f * size,  0.5f * size), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f) },
					PositionNormalTexCoord{ vec3(-0.5f * size,  0.5f * size,  0.5f * size), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f) },

					// Right
					PositionNormalTexCoord{ vec3( 0.5f * size, -0.5f * size,  0.5f * size), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f) },
					PositionNormalTexCoord{ vec3( 0.5f * size, -0.5f * size, -0.5f * size), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f) },
					PositionNormalTexCoord{ vec3( 0.5f * size,  0.5f * size, -0.5f * size), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f) },
					PositionNormalTexCoord{ vec3( 0.5f * size,  0.5f * size,  0.5f * size), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f) },

					// Back
					PositionNormalTexCoord{ vec3(-0.5f * size, -0.5f * size, -0.5f * size), vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f) },
					PositionNormalTexCoord{ vec3(-0.5f * size,  0.5f * size, -0.5f * size), vec3(0.0f, 0.0f, -1.0f), vec2(1.0f, 0.0f) },
					PositionNormalTexCoord{ vec3( 0.5f * size,  0.5f * size, -0.5f * size), vec3(0.0f, 0.0f, -1.0f), vec2(1.0f, 1.0f) },
					PositionNormalTexCoord{ vec3( 0.5f * size, -0.5f * size, -0.5f * size), vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 1.0f) },

					// Left
					PositionNormalTexCoord{ vec3(-0.5f * size, -0.5f * size,  0.5f * size), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f) },
					PositionNormalTexCoord{ vec3(-0.5f * size,  0.5f * size,  0.5f * size), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f) },
					PositionNormalTexCoord{ vec3(-0.5f * size,  0.5f * size, -0.5f * size), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f) },
					PositionNormalTexCoord{ vec3(-0.5f * size, -0.5f * size, -0.5f * size), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f) },

					// Bottom
					PositionNormalTexCoord{ vec3(-0.5f * size, -0.5f * size,  0.5f * size), vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 0.0f) },
					PositionNormalTexCoord{ vec3(-0.5f * size, -0.5f * size, -0.5f * size), vec3(0.0f, -1.0f, 0.0f), vec2(1.0f, 0.0f) },
					PositionNormalTexCoord{ vec3( 0.5f * size, -0.5f * size, -0.5f * size), vec3(0.0f, -1.0f, 0.0f), vec2(1.0f, 1.0f) },
					PositionNormalTexCoord{ vec3( 0.5f * size, -0.5f * size,  0.5f * size), vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 1.0f) },

					// Top
					PositionNormalTexCoord{ vec3(-0.5f * size,  0.5f * size,  0.5f * size), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f) },
					PositionNormalTexCoord{ vec3( 0.5f * size,  0.5f * size,  0.5f * size), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f) },
					PositionNormalTexCoord{ vec3( 0.5f * size,  0.5f * size, -0.5f * size), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f) },
					PositionNormalTexCoord{ vec3(-0.5f * size,  0.5f * size, -0.5f * size), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f) },
				}),
				mIndexBuffer({
					0, 1, 2, 0, 2, 3,
					4, 5, 6, 4, 6, 7,
					8, 9, 10, 8, 10, 11,
					12, 13, 14, 12, 14, 15,
					16, 17, 18, 16, 18, 19,
					20, 21, 22, 20, 22, 23
				})
			{
				mVAO.attach(mVertexBuffer);
				mVAO.attach(mIndexBuffer);
			}

			void Cube::draw() {
				mVAO.draw();
			}
		}
	}
}
