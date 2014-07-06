#include "render/shape/cube.h"

namespace overdrive {
	namespace render {
		namespace shape {
			Cube::Cube(float size):
				mNumElements(0)
			{
				float halfSide = size * 0.5f;

				float vertices[] = {
					// Front
					-halfSide, -halfSide, halfSide,
					 halfSide, -halfSide, halfSide,
					 halfSide,  halfSide, halfSide,
					-halfSide,  halfSide, halfSide,

					// Right
					halfSide, -halfSide,  halfSide,
					halfSide, -halfSide, -halfSide,
					halfSide,  halfSide, -halfSide,
					halfSide,  halfSide,  halfSide,

					// Back
					-halfSide, -halfSide, -halfSide,
					-halfSide,  halfSide, -halfSide,
					 halfSide,  halfSide, -halfSide,
					 halfSide, -halfSide, -halfSide,

					// Left
					-halfSide, -halfSide,  halfSide,
					-halfSide,  halfSide,  halfSide,
					-halfSide,  halfSide, -halfSide,
					-halfSide, -halfSide, -halfSide,

					// Bottom
					-halfSide, -halfSide,  halfSide,
					-halfSide, -halfSide, -halfSide,
					 halfSide, -halfSide, -halfSide,
					 halfSide, -halfSide,  halfSide,

					// Top
					-halfSide, halfSide,  halfSide,
					 halfSide, halfSide,  halfSide,
					 halfSide, halfSide, -halfSide,
					-halfSide, halfSide, -halfSide
				};

				float normals[] = {
					// Front
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f,

					// Right
					1.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f,

					// Back
					0.0f, 0.0f, -1.0f,
					0.0f, 0.0f, -1.0f,
					0.0f, 0.0f, -1.0f,
					0.0f, 0.0f, -1.0f,

					// Left
					-1.0f, 0.0f, 0.0f,
					-1.0f, 0.0f, 0.0f,
					-1.0f, 0.0f, 0.0f,
					-1.0f, 0.0f, 0.0f,

					// Bottom
					0.0f, -1.0f, 0.0f,
					0.0f, -1.0f, 0.0f,
					0.0f, -1.0f, 0.0f,
					0.0f, -1.0f, 0.0f,

					// Top
					0.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 0.0f
				};

				float texCoords[] = {
					// Front
					0.0f, 0.0f,
					1.0f, 0.0f,
					1.0f, 1.0f,
					0.0f, 1.0f,

					// Right
					0.0f, 0.0f,
					1.0f, 0.0f,
					1.0f, 1.0f,
					0.0f, 1.0f,

					// Back
					0.0f, 0.0f,
					1.0f, 0.0f,
					1.0f, 1.0f,
					0.0f, 1.0f,

					// Left
					0.0f, 0.0f,
					1.0f, 0.0f,
					1.0f, 1.0f,
					0.0f, 1.0f,

					// Bottom
					0.0f, 0.0f,
					1.0f, 0.0f,
					1.0f, 1.0f,
					0.0f, 1.0f,

					// Top
					0.0f, 0.0f,
					1.0f, 0.0f,
					1.0f, 1.0f,
					0.0f, 1.0f
				};

				GLuint indices[] = {
					0, 1, 2, 
					0, 2, 3,

					4, 5, 6, 
					4, 6, 7,

					8, 9, 10, 
					8, 10, 11,

					12, 13, 14, 
					12, 14, 15,

					16, 17, 18, 
					16, 18, 19,

					20, 21, 22, 
					20, 22, 23
				};

				mNumElements = sizeof(indices) / sizeof(indices[0]);

				GLuint vao = 0;
				glGenVertexArrays(1, &vao);
				mVertexArray = std::move(vao);

				glBindVertexArray(mVertexArray);

				{
					GLuint buffers[4];
					glGenBuffers(4, buffers);
					mVertices = std::move(buffers[0]);
					mNormals = std::move(buffers[1]);
					mTexCoords = std::move(buffers[2]);
					mIndices = std::move(buffers[3]);

					glBindBuffer(GL_ARRAY_BUFFER, mVertices);
					glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
					glVertexAttribPointer(
						0,			// location 0
						3,			// 3 elements
						GL_FLOAT,	// of type float
						GL_FALSE,	// not normalized
						0,			// no stride
						nullptr		// no corresponding buffer (data is bound)
					);
					glEnableVertexAttribArray(0); //enable vertex attribute 0 -- position

					glBindBuffer(GL_ARRAY_BUFFER, mNormals);
					glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
					glVertexAttribPointer(
						1,			// location 1
						3,			// 3 elements
						GL_FLOAT,	// of type float
						GL_FALSE,	// not normalized
						0,			// no stride
						nullptr		// no corresponding buffer (data is bound)
					);
					glEnableVertexAttribArray(1); //enable vertex attribute 1 -- normal

					glBindBuffer(GL_ARRAY_BUFFER, mTexCoords);
					glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
					glVertexAttribPointer(
						2,			// location 2
						2,			// 2 elements
						GL_FLOAT,	// of type float 
						GL_FALSE,	// not normalized
						0,			// no stride
						nullptr		// no corresponding buffer (data is bound)
					);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
				}

				glBindVertexArray(0);
			}

			void Cube::draw() const {
				glBindVertexArray(mVertexArray);
				glDrawElements(GL_TRIANGLES, mNumElements, GL_UNSIGNED_INT, nullptr);
			}
		}
	}
}