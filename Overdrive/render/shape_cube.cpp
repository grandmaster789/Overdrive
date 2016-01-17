#include "stdafx.h"
#include "shape_cube.h"

namespace overdrive {
	namespace render {
		namespace shape {
			Cube::Cube(float size):
				mVertexBuffer({
					// Front
					attributes::PositionNormalTexCoord { glm::vec3(-0.5f * size, -0.5f * size,  0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
					attributes::PositionNormalTexCoord { glm::vec3( 0.5f * size, -0.5f * size,  0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
					attributes::PositionNormalTexCoord { glm::vec3( 0.5f * size,  0.5f * size,  0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
					attributes::PositionNormalTexCoord { glm::vec3(-0.5f * size,  0.5f * size,  0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

					// Right
					attributes::PositionNormalTexCoord{ glm::vec3( 0.5f * size, -0.5f * size,  0.5f * size), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3( 0.5f * size, -0.5f * size, -0.5f * size), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3( 0.5f * size,  0.5f * size, -0.5f * size), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3( 0.5f * size,  0.5f * size,  0.5f * size), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) },

					// Back
					attributes::PositionNormalTexCoord{ glm::vec3(-0.5f * size, -0.5f * size, -0.5f * size), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3(-0.5f * size,  0.5f * size, -0.5f * size), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3( 0.5f * size,  0.5f * size, -0.5f * size), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3( 0.5f * size, -0.5f * size, -0.5f * size), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f) },

					// Left
					attributes::PositionNormalTexCoord{ glm::vec3(-0.5f * size, -0.5f * size,  0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3(-0.5f * size,  0.5f * size,  0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3(-0.5f * size,  0.5f * size, -0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3(-0.5f * size, -0.5f * size, -0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

					// Bottom
					attributes::PositionNormalTexCoord{ glm::vec3(-0.5f * size, -0.5f * size,  0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3(-0.5f * size, -0.5f * size, -0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3( 0.5f * size, -0.5f * size, -0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3( 0.5f * size, -0.5f * size,  0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

					// Top
					attributes::PositionNormalTexCoord{ glm::vec3(-0.5f * size,  0.5f * size,  0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3( 0.5f * size,  0.5f * size,  0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3( 0.5f * size,  0.5f * size, -0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
					attributes::PositionNormalTexCoord{ glm::vec3(-0.5f * size,  0.5f * size, -0.5f * size), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
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
