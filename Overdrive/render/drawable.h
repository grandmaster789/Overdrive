#ifndef OVERDRIVE_RENDER_DRAWABLE_H
#define OVERDRIVE_RENDER_DRAWABLE_H

#include "render/drawable.h"
#include "util/unique_resource.h"
#include "opengl.h"

namespace overdrive {
	namespace render {
		class Drawable {
		public:
			Drawable();

			virtual void draw() const = 0;

		protected:
			static void deleteVertexArray(GLuint vao);
			static void deleteBuffer(GLuint vbo);

			UniqueResource<GLuint, void(*)(GLuint)> mVertexArray;

			UniqueResource<GLuint, void(*)(GLuint)> mVertices;
			UniqueResource<GLuint, void(*)(GLuint)> mIndices;
			UniqueResource<GLuint, void(*)(GLuint)> mNormals;
			UniqueResource<GLuint, void(*)(GLuint)> mTexCoords;
		};
	}
}

#endif
