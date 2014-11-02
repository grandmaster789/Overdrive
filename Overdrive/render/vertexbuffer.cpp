#include "render/vertexbuffer.h"
#include "util/checkGL.h"

namespace overdrive {
	namespace render {
		const void* VertexData::getRaw() const {
			return static_cast<const void*>(mData.data());
		}

		size_t VertexData::getSize() const {
			return mData.size();
		}

		VertexBuffer::VertexBuffer():
			mHandle(0)
		{
			glGenBuffers(1, &mHandle);
			CHECK_GL_STATE;
		}

		VertexBuffer::VertexBuffer(
			const void* sourceData,
			size_t numBytes,
			eBufferUsage usage
		) :
			mHandle(0) 
		{
			glGenBuffers(1, &mHandle);
			CHECK_GL_STATE;

			setData(sourceData, numBytes, usage);
		}

		VertexBuffer::~VertexBuffer() {
			if (mHandle)
				glDeleteBuffers(1, &mHandle);
		}

		GLuint VertexBuffer::getHandle() const {
			return mHandle;
		}

		void VertexBuffer::setData(
			const void* sourceData, 
			size_t numBytes, 
			eBufferUsage usage
		) {
			assert(mHandle != 0);

			glBindBuffer(GL_ARRAY_BUFFER, mHandle);
			glBufferData(GL_ARRAY_BUFFER, numBytes, sourceData, static_cast<GLenum>(usage));
		}

		void VertexBuffer::setSubData(
			size_t offset,
			size_t numBytes,
			const void* sourceData
		) {
			assert(mHandle != 0);

			glBindBuffer(GL_ARRAY_BUFFER, mHandle);
			glBufferSubData(GL_ARRAY_BUFFER, offset, numBytes, sourceData);
		}

		void VertexBuffer::getSubData(
			size_t offset, 
			size_t numBytes,
			void* result
		) {
			assert(mHandle != 0);

			glBindBuffer(GL_ARRAY_BUFFER, mHandle);
			glGetBufferSubData(GL_ARRAY_BUFFER, offset, numBytes, result);
		}
	}
}