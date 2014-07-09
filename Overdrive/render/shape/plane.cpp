#include "render/shape/plane.h"
#include "opengl.h"
#include <memory>

namespace overdrive {
	namespace render {
		namespace shape {
			Plane::Plane(
				float xSize,
				float zSize,
				size_t numXDivs,
				size_t numZDivs
			) {
				if (numXDivs < 1)
					numXDivs = 1;

				if (numZDivs < 1)
					numZDivs = 1;

				size_t numVertices = (numXDivs + 1) * (numZDivs + 1);
				mNumFaces = numXDivs * numZDivs;

				std::unique_ptr<GLfloat[]> vertices(new GLfloat[3 * numVertices]);
				std::unique_ptr<GLfloat[]> normals(new GLfloat[3 * numVertices]);
				std::unique_ptr<GLfloat[]> texCoords(new GLfloat[2 * numVertices]);
				std::unique_ptr<GLuint[]> indices(new GLuint[6 * mNumFaces]);

				float halfX = xSize * 0.5f;
				float halfZ = zSize * 0.5f;
				
				float vi = zSize / numZDivs;
				float vj = xSize / numXDivs;

				float ti = 1.0f / numZDivs;
				float tj = 1.0f / numXDivs;

				float x;
				float z;

				size_t vx = 0;
				size_t tx = 0;

				// generate vertices, normals and texture coordinates
				for (size_t i = 0; i <= numZDivs; ++i) {
					z = vi * i - halfZ;
					
					for (size_t j = 0; j <= numXDivs; ++j) {
						x = vj * j - halfX;

						vertices[vx + 0] = x;
						vertices[vx + 1] = 0.0f;
						vertices[vx + 2] = z;

						normals[vx + 0] = 0.0f;
						normals[vx + 1] = 1.0f;
						normals[vx + 2] = 0.0f;

						vx += 3;

						texCoords[tx + 0] = j * tj;
						texCoords[tx + 1] = i * ti;

						tx += 2;
					}
				}

				// create indices
				size_t row;
				size_t nextRow;
				size_t idx = 0;

				for (size_t i = 0; i < numZDivs; ++i) {
					row = i * (numXDivs  + 1);
					nextRow = (i + 1) * (numXDivs + 1);

					for (size_t j = 0; j < numXDivs; ++j) {
						// two triangles
						indices[idx + 0] = row + j;
						indices[idx + 1] = nextRow + j;
						indices[idx + 2] = nextRow + j + 1;

						indices[idx + 3] = row + j;
						indices[idx + 4] = nextRow + j + 1;
						indices[idx + 5] = row + j + 1;

						idx += 6;
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

			void Plane::draw() const {
				glBindVertexArray(mVertexArray);
				glDrawElements(GL_TRIANGLES, 6 * mNumFaces, GL_UNSIGNED_INT, nullptr);
			}
		}
	}
}