#include "stdafx.h"
#include "vertexarray.h"
#include "gltypes.h"

namespace overdrive {
	namespace render {
		VertexArray::VertexArray() {
			glGenVertexArrays(1, &mHandle);
		}

		VertexArray::~VertexArray() {
			if (mHandle)
				glDeleteVertexArrays(1, &mHandle);
		}

		GLuint VertexArray::getHandle() const {
			return mHandle;
		}

		void VertexArray::bind() {
			glBindVertexArray(mHandle);
		}

		void VertexArray::unbind() {
			glBindVertexArray(0);
		}

		void VertexArray::draw(ePrimitives mode_) {
			bind();

			GLenum mode = static_cast<GLenum>(mode_);

			if (mIndexBufferType == 0)
				glDrawArrays(mode, 0, mVertexBufferSize);
			else
				glDrawElements(mode, mIndexBufferSize, mIndexBufferType, nullptr);

			unbind();
		}

		void VertexArray::drawInstanced(GLsizei primitiveCount, ePrimitives mode_) {
			bind();

			GLenum mode = static_cast<GLenum>(mode_);

			if (mIndexBufferType == 0)
				glDrawArraysInstanced(
					mode, 
					0, 
					mVertexBufferSize, 
					primitiveCount
				);
			else
				glDrawElementsInstanced(
					mode, 
					mIndexBufferSize, 
					mIndexBufferType, 
					nullptr, 
					primitiveCount
				);

			unbind();
		}

		void VertexArray::drawRange(GLuint begin, GLuint end, ePrimitives mode_) {
			bind();

			assert(end >= begin);

			GLenum mode = static_cast<GLenum>(mode_);
			GLsizei count = end - begin;
			GLvoid* offset = (GLvoid*)(getTypeSize(mIndexBufferType) * begin);

			if (mIndexBufferType == 0)
				glDrawArrays(
					mode,
					begin,
					count
				);
			else
				glDrawRangeElements(
					mode,
					0,
					mVertexBufferSize,
					count,
					mIndexBufferType,
					offset
				);

			unbind();
		}

		std::ostream& operator << (std::ostream& os, const ePrimitives& value) {
			switch (value) {
			case ePrimitives::POINTS:						os << "points"; break;

			case ePrimitives::LINE_STRIP:					os << "line strip"; break;
			case ePrimitives::LINE_LOOP:					os << "line loop"; break;
			case ePrimitives::LINES:						os << "lines"; break;
			case ePrimitives::LINE_STRIP_ADJACENCY:			os << "line strip adjacency"; break;
			case ePrimitives::LINES_ADJACENCY:				os << "lines adjacency"; break;

			case ePrimitives::TRIANGLE_STRIP:				os << "triangle strip"; break;
			case ePrimitives::TRIANGLE_FAN:					os << "triangle fan"; break;
			case ePrimitives::TRIANGLES:					os << "triangles"; break;
			case ePrimitives::TRIANGLE_STRIP_ADJACENCY:		os << "triangle strip adjacency"; break;
			case ePrimitives::TRIANGLES_ADJACENCY:			os << "triangles adjacency"; break;

			case ePrimitives::PATCHES:						os << "patches"; break;
			default:
				os << "Unknown primitive type: " << static_cast<std::underlying_type<ePrimitives>::type>(value);
			}
			return os;
		}
	}
}