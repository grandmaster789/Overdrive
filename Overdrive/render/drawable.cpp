#include "render/drawable.h"

namespace overdrive {
	namespace render {
		Drawable::Drawable():
			mVertexArray(0, &deleteVertexArray, false),
			
			mVertices(0, &deleteBuffer, false),
			mIndices(0, &deleteBuffer, false),
			mNormals(0, &deleteBuffer, false),
			mTangents(0, &deleteBuffer, false),
			mTexCoords(0, &deleteBuffer, false)
		{
		}

		void Drawable::deleteVertexArray(GLuint vao) {
			glDeleteVertexArrays(1, &vao);
		}

		void Drawable::deleteBuffer(GLuint vbo) {
			glDeleteBuffers(1, &vbo);
		}
	}
}