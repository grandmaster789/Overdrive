#pragma once

#include "vertexbuffer.h"
#include "indexbuffer.h"
#include <ostream>

namespace overdrive {
	namespace render {
		enum class ePrimitives : GLenum {
			POINTS = GL_POINTS,

			LINE_STRIP = GL_LINE_STRIP,
			LINE_LOOP = GL_LINE_LOOP,
			LINES = GL_LINES,
			LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
			LINES_ADJACENCY = GL_LINES_ADJACENCY,

			TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
			TRIANGLE_FAN = GL_TRIANGLE_FAN,
			TRIANGLES = GL_TRIANGLES,
			TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
			TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,

			PATCHES = GL_PATCHES
		};
		/*
			[TODO] Figure out how the draw indirect is supposed to work
			[TODO] Figure out how the multidrawarray is supposed to work
			[TODO] Perhaps get rid of the branch in :draw*() and make both variations explicit
		*/
		// http://docs.gl/gl4/glDrawArrays
		// http://docs.gl/gl4/glDrawArraysInstanced
		// http://docs.gl/gl4/glDrawElements
		// http://docs.gl/gl4/glDrawElementsInstanced
		// http://docs.gl/gl4/glDrawRangeElements
		class VertexArray {
		public:
			VertexArray();
			~VertexArray();

			VertexArray(const VertexArray&) = delete;
			VertexArray(VertexArray&&) = delete;
			VertexArray& operator = (const VertexArray&) = delete;
			VertexArray& operator = (VertexArray&&) = delete;

			GLuint getHandle() const;

			void bind();
			void unbind();

			template <typename Seq>
			void attach(VertexBuffer<Seq>& vertexbuffer);

			template <typename T>
			void attach(IndexBuffer<T>& indexbuffer);

			void draw(ePrimitives mode = ePrimitives::TRIANGLES);
			void drawInstanced(GLsizei primitiveCount, ePrimitives mode = ePrimitives::TRIANGLES);
			void drawRange(GLuint begin, GLuint end, ePrimitives mode = ePrimitives::TRIANGLES); // [NOTE] not entirely sure I got this one right, needs to be tested

			template <typename T>
			void draw(size_t numElements, T* indexbuffer, ePrimitives mode); // draw with raw index buffer

		private:
			GLuint mHandle;

			GLsizei mVertexBufferSize = 0;
			GLsizei mIndexBufferSize = 0;
			GLenum mIndexBufferType = 0;
		};

		std::ostream& operator << (std::ostream& os, const ePrimitives& value);
	}
}

#include "vertexarray.inl"