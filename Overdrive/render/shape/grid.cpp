#include "render/shape/grid.h"
#include "opengl.h"
#include <numeric>
#include <memory>


namespace overdrive {
	namespace render {
		namespace shape {
			Grid::Grid(
				float xSize,
				float zSize,
				size_t numXDivs,
				size_t numZDivs
			) {
				// enforce at least outer edge lines
				if (numXDivs < 2)
					numXDivs = 2;

				if (numZDivs < 2)
					numZDivs = 2;

				size_t numVertices = (numXDivs * 2) + (numXDivs * 2);
				mNumIndices = 2 * numVertices;

				std::unique_ptr<GLfloat[]> vertices(new GLfloat[3 * numVertices]);
				std::unique_ptr<GLuint[]> indices(new GLuint[mNumIndices]);

				float halfX = xSize * 0.5f;
				float halfZ = zSize * 0.5f;

				float vi = zSize / numZDivs;
				float vj = xSize / numXDivs;

				float x;
				float z;

				size_t vx = 0;

				// create vertices, normals, texcoords

				for (size_t i = 0; i < numZDivs; ++i) {
					z = i * vi - halfZ;
					
					vertices[vx + 0] = -halfX;
					vertices[vx + 1] = 0.0f;
					vertices[vx + 2] = z;

					vertices[vx + 3] = halfX;
					vertices[vx + 4] = 0.0f;
					vertices[vx + 5] = z;

					vx += 6;
				}

				for (size_t j = 0; j < numXDivs; ++j) {
					x = j * vj - halfX;

					vertices[vx + 0] = x;
					vertices[vx + 1] = 0.0f;
					vertices[vx + 2] = -halfZ;

					vertices[vx + 3] = x;
					vertices[vx + 4] = 0.0f;
					vertices[vx + 5] = halfZ;

					vx += 6;
				}

				// create indices
				for (size_t i = 0; i < mNumIndices; ++i)
					indices[i] = i;

				// create VBO's to hold the data on the gpu
				GLuint buffers[2];
				glGenBuffers(2, buffers);
				mVertices = std::move(buffers[0]);
				mIndices = std::move(buffers[1]);

				glBindBuffer(GL_ARRAY_BUFFER, mVertices);
				glBufferData(GL_ARRAY_BUFFER, (3 * numVertices) * sizeof(GLfloat), vertices.get(), GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumIndices * sizeof(GLuint), indices.get(), GL_STATIC_DRAW);

				// create a VAO
				GLuint vao = 0;
				glGenVertexArrays(1, &vao);
				mVertexArray = std::move(vao);

				glBindVertexArray(mVertexArray);

					glEnableVertexAttribArray(0);  // Vertex position
					glBindBuffer(GL_ARRAY_BUFFER, mVertices);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);

				glBindVertexArray(0);
			}

			void Grid::draw() const {
				glBindVertexArray(mVertexArray);
				glDrawElements(GL_LINES, mNumIndices, GL_UNSIGNED_INT, 0);
			}
		}
	}
}