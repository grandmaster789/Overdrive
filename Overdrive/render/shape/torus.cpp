#include "render/shape/torus.h"
#include "math/constants.h"
#include "math/util.h"
#include "opengl.h"
#include <memory>
#include <cmath>

namespace overdrive {
	namespace render {
		namespace shape {
			Torus::Torus(
				float outerRadius,
				float innerRadius,
				size_t numRings,
				size_t numSides
			):
				mNumRings(numRings),
				mNumSides(numSides)
			{
				if (mNumRings < 2)
					mNumRings = 2;

				if (mNumSides < 3)
					mNumSides = 3;

				mNumFaces = mNumRings * mNumSides;

				size_t numVertices = mNumSides * (mNumRings + 1);

				// allocate some buffers to store the raw coordinate data
				std::unique_ptr<GLfloat[]> vertices(new GLfloat[3 * numVertices]);
				std::unique_ptr<GLfloat[]> normals(new GLfloat[3 * numVertices]);
				std::unique_ptr<GLfloat[]> texCoords(new GLfloat[2 * numVertices]);
				std::unique_ptr<GLuint[]> indices(new GLuint[6 * mNumFaces]);

				// procedurally generate the data
				{
					using math::TWO_PI;
					using math::square;
					using std::sin;
					using std::cos;
					using std::sqrt;

					float side = (TWO_PI / mNumSides);
					float ring = (TWO_PI / mNumRings);

					// start with the vtx/normal/tx data
					size_t vtx = 0; // <- vertex element
					size_t tx = 0; // <- texCoord element

					for (size_t currentRing = 0; currentRing <= mNumRings; ++currentRing) {
						float x = currentRing * ring;
						float sx = sin(x);
						float cx = cos(x);

						for (size_t currentSide = 0; currentSide < mNumSides; ++currentSide) {
							// vertex/normal calculation
							float y = currentSide * side;
							float sy = sin(y);
							float cy = cos(y);

							float radius = (outerRadius + innerRadius * cy);

							vertices[vtx + 0] = radius * cx;
							vertices[vtx + 1] = radius * sx;
							vertices[vtx + 2] = innerRadius * sy;

							normals[vtx + 0] = cx * cy * radius;
							normals[vtx + 1] = sx * cy * radius;
							normals[vtx + 2] = sy * radius;

							float len = sqrt(
								square(normals[vtx + 0]) +
								square(normals[vtx + 1]) +
								square(normals[vtx + 2])
							);

							normals[vtx + 0] /= len;
							normals[vtx + 1] /= len;
							normals[vtx + 2] /= len;

							vtx += 3;

							// texcoord calculation
							texCoords[tx + 0] = x / TWO_PI;
							texCoords[tx + 1] = y / TWO_PI;

							tx += 2;
						}
					}

					// generate indices
					size_t idx = 0;
					
					for (size_t currentRing = 0; currentRing < mNumRings; ++currentRing) {
						size_t startRing = currentRing * mNumSides;
						size_t nextRing = (currentRing + 1) * mNumSides;

						for (size_t currentSide = 0; currentSide < mNumSides; ++currentSide) {
							size_t nextSide = (currentSide + 1) % mNumSides;

							// produce two triangles
							indices[idx + 0] = (startRing + currentSide);
							indices[idx + 1] = (nextRing + currentSide);
							indices[idx + 2] = (nextRing + nextSide);

							indices[idx + 3] = (startRing + currentSide);
							indices[idx + 4] = (nextRing + nextSide);
							indices[idx + 5] = (startRing + nextSide);

							idx += 6;
						}
					}
				}

				// create VBO's to hold the data on the gpu
				GLuint buffers[4];
				glGenBuffers(4, buffers);
				mVertices = std::move(buffers[0]);
				mNormals = std::move(buffers[1]);
				mTexCoords = std::move(buffers[2]);
				mIndices = std::move(buffers[3]);

				glBindBuffer(GL_ARRAY_BUFFER, mVertices);
				glBufferData(GL_ARRAY_BUFFER, (3 * numVertices) * sizeof(GLfloat), vertices.get(), GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, mNormals);
				glBufferData(GL_ARRAY_BUFFER, (3 * numVertices) * sizeof(GLfloat), normals.get(), GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, mTexCoords);
				glBufferData(GL_ARRAY_BUFFER, (2 * numVertices) * sizeof(GLfloat), texCoords.get(), GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * mNumFaces * sizeof(GLuint), indices.get(), GL_STATIC_DRAW);

				// create a VAO
				GLuint vao = 0;
				glGenVertexArrays(1, &vao);
				mVertexArray = std::move(vao);

				glBindVertexArray(mVertexArray);

					glEnableVertexAttribArray(0);  // Vertex position
					glBindBuffer(GL_ARRAY_BUFFER, mVertices);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

					glEnableVertexAttribArray(1);  // Vertex normal
					glBindBuffer(GL_ARRAY_BUFFER, mNormals);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

					glBindBuffer(GL_ARRAY_BUFFER, mTexCoords);
					glEnableVertexAttribArray(2);  // Texture coords
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);

				glBindVertexArray(0);
			}

			void Torus::draw() const {
				glBindVertexArray(mVertexArray);
				glDrawElements(GL_TRIANGLES, 6 * mNumFaces, GL_UNSIGNED_INT, nullptr);
			}
		}
	}
}