#ifndef OVERDRIVE_RENDER_DRAWABLE_H
#define OVERDRIVE_RENDER_DRAWABLE_H

#include "render/drawable.h"
#include "util/unique_resource.h"
#include "opengl.h"

namespace overdrive {
	namespace render {
		typedef UniqueResource<GLuint, void(*)(GLuint)> ManagedGLResource;

		class Drawable {
		public:
			Drawable();

			virtual void draw() const = 0;

		protected:
			static void deleteVertexArray(GLuint vao);
			static void deleteBuffer(GLuint vbo);

			ManagedGLResource mVertexArray;

			ManagedGLResource mVertices;
			ManagedGLResource mIndices;
			ManagedGLResource mNormals;
			ManagedGLResource mTangents;
			ManagedGLResource mTexCoords;
		};
	}
}

#endif
