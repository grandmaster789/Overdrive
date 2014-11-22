#ifndef OVERDRIVE_RENDER_VERTEXBUFFER_INL
#define OVERDRIVE_RENDER_VERTEXBUFFER_INL

#include "render/vertexbuffer.h"

namespace overdrive {
	namespace render {
		template <typename T, int I>
		VertexBuffer<T, I>::VertexBuffer(eBufferUsage usage) :
			mBufferUsage(usage),
			mNumBytes(0),
			mHandle(0)
		{
			glGenBuffers(1, &mHandle);
			CHECK_GL_STATE;
		}

		template <typename T, int I>
		VertexBuffer<T, I>::~VertexBuffer() {
			if (mHandle)
				glDeleteBuffers(1, &mHandle);
		}

		template <typename T, int I>
		GLuint VertexBuffer<T, I>::getHandle() const {
			return mHandle;
		}

		template <typename T, int I>
		size_t VertexBuffer<T, I>::getNumBytes() const {
			return mNumBytes;
		}

		template <typename T, int I>
		size_t VertexBuffer<T, I>::getNumElements() const {
			return mNumBytes / (I * sizeof(T));
		}

		template <typename T, int I>
		void VertexBuffer<T, I>::setData(
			const T* data,
			size_t numBytes
		) {
			assert((numBytes / sizeof(T))  % I == 0); // the number of elements should be a multiple of I

			mNumBytes = numBytes;

			glBindBuffer(GL_ARRAY_BUFFER, mHandle);
			glBufferData(
				GL_ARRAY_BUFFER, 
				numBytes,
				data, 
				static_cast<GLenum>(mBufferUsage)
			);

			CHECK_GL_STATE;
		}

		template <typename T, int I>
		void VertexBuffer<T, I>::setData(const std::vector<T>& data) {
			setData(data.data(), data.size() * sizeof(T));
		}

		// -------------------- Convenience functions ---------------------------------

		template <typename T> 
		void push(std::vector<T>& buffer, const T& x) {
			buffer.push_back(x);
		}

		template <typename T> 
		void push(std::vector<T>& buffer, const T& x, const T& y) {
			buffer.push_back(x);
			buffer.push_back(y);
		}

		template <typename T> 
		void push(std::vector<T>& buffer, const T& x, const T& y, const T& z) {
			buffer.push_back(x);
			buffer.push_back(y);
			buffer.push_back(z);
		}
	}
}

#endif
