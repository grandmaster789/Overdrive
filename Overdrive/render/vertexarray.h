#ifndef OVERDRIVE_RENDER_VERTEXARRAY_H
#define OVERDRIVE_RENDER_VERTEXARRAY_H

#include "opengl.h"
#include "render/vertexbuffer.h"

namespace overdrive {
	namespace render {
		class VertexArray {
		public:
			VertexArray();
			~VertexArray();

			VertexArray(const VertexArray&) = delete;
			VertexArray& operator = (const VertexArray&) = delete;

			GLuint getHandle() const;

			template <typename T, int I>
			void bindAttribute(
				GLuint location,
				const VertexBuffer<T, I>& buffer,
				GLsizei stride = 0,
				const GLvoid* offset = nullptr
			);

			void bindElements(const VertexBuffer<GLuint, 1>& indexBuffer);

		private:
			GLuint mHandle;
		};

		// [omg] this probably needs to move to util
		template <typename T>
		struct GLType {
			static GLenum type;
		};

		template <> GLenum GLType<float>::type = GL_FLOAT;
		template <> GLenum GLType<double>::type = GL_DOUBLE;
		template <> GLenum GLType<char>::type = GL_BYTE;
		template <> GLenum GLType<short>::type = GL_SHORT;
		template <> GLenum GLType<int>::type = GL_INT;
		template <> GLenum GLType<unsigned int>::type = GL_UNSIGNED_INT;
		template <> GLenum GLType<unsigned short>::type = GL_UNSIGNED_SHORT;
		template <> GLenum GLType<unsigned char>::type = GL_UNSIGNED_BYTE;

		/* -------------- Implementation --------------------- */
		template <typename T, int I>
		void VertexArray::bindAttribute(
			GLuint location,
			const VertexBuffer<T, I>& buffer,
			GLsizei stride,
			const GLvoid* offset
		) {
			glBindVertexArray(mHandle);

			glBindBuffer(GL_ARRAY_BUFFER, buffer.getHandle());
			glVertexAttribPointer(
				location,
				I,
				GLType<T>::type,
				GL_FALSE, // never normalized
				stride,
				offset
			);
			glEnableVertexAttribArray(location);
		}
	}
}

#endif
