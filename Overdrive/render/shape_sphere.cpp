#include "stdafx.h"
#include "shape_sphere.h"
#include <boost/math/constants/constants.hpp>

namespace overdrive {
	namespace render {
		namespace shape {
			Sphere::Sphere(
				float radius,
				unsigned int slices,
				unsigned int stacks
			):
				mVertexBuffer((slices + 1) * (stacks + 1)),
				mIndexBuffer((slices * 2 * (stacks - 1)) * 3)
			{
				{
					using attributes::PositionNormalTexCoord;
					using boost::math::float_constants::pi;

					float theta;
					float phi;
					
					float thetaFactor = 2.0f * pi / slices;
					float phiFactor = pi / stacks;

					glm::vec3 position;
					glm::vec3 normal;
					glm::vec2 texCoord;

					auto vertices = mVertexBuffer.map();

					unsigned int index = 0;

					for (unsigned int i = 0; i <= slices; ++i) {
						theta = i * thetaFactor;

						texCoord.s = static_cast<float>(i) / stacks;

						for (unsigned int j = 0; j <= stacks; ++j) {
							phi = j * phiFactor;

							texCoord.t = static_cast<float>(j) / stacks;

							normal.x = sinf(phi) * cosf(theta);
							normal.y = sinf(phi) * sinf(theta);
							normal.z = cosf(phi);

							position = normal * radius;

							vertices[index++] = PositionNormalTexCoord{ position, normal, texCoord };
						}
					}
				}

				{
					auto indices = mIndexBuffer.map();

					unsigned int index = 0;

					for (GLuint i = 0; i < slices; ++i) {
						GLuint stackStart = i * (stacks + 1);
						GLuint nextStackStart = (i + 1) * (stacks + 1);

						for (GLuint j = 0; j < stacks; ++j) {
							if (j == 0) {
								indices[index    ] = stackStart;
								indices[index + 1] = stackStart + 1;
								indices[index + 2] = nextStackStart + 1;

								index += 3;
							}
							else if (j == (stacks - 1)) {
								indices[index    ] = stackStart + j;
								indices[index + 1] = stackStart + j + 1;
								indices[index + 2] = nextStackStart + j;

								index += 3;
							}
							else {
								indices[index    ] = stackStart + j;
								indices[index + 1] = stackStart + j + 1;
								indices[index + 2] = nextStackStart + j + 1;
								indices[index + 3] = nextStackStart + j;
								indices[index + 4] = stackStart + j;
								indices[index + 5] = nextStackStart + j + 1;

								index += 6;
							}
						}
					}
				}

				mVAO.attach(mIndexBuffer);
				mVAO.attach(mVertexBuffer);
			}

			void Sphere::draw() {
				mVAO.draw();
			}
		}
	}
}