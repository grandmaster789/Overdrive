#ifndef OVERDRIVE_RENDER_VERTEXBUFFER_H
#define OVERDRIVE_RENDER_VERTEXBUFFER_H

#include <functional>
#include <cstdint>
#include <vector>
#include "opengl.h"

namespace overdrive {
	namespace render {
		enum class eBufferUsage: GLenum {
			STATIC_DRAW = GL_STATIC_DRAW,
			STATIC_READ = GL_STATIC_READ,
			STATIC_COPY = GL_STATIC_COPY,

			DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
			DYNAMIC_READ = GL_DYNAMIC_READ,
			DYNAMIC_COPY = GL_DYNAMIC_COPY,

			STREAMING_DRAW = GL_STREAM_DRAW,
			STREAMING_READ = GL_STREAM_READ,
			STREAMING_COPY = GL_STREAM_COPY
		};

		enum class eElementType : GLenum {
			BYTE = GL_BYTE,
			SHORT = GL_SHORT,
			INT = GL_INT,

			UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
			UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
			UNSIGNED_INT = GL_UNSIGNED_INT,

			FLOAT = GL_FLOAT,
			DOUBLE = GL_DOUBLE,			
		};

		class VertexData {
		public:
			template <typename T>
			void add(const T& value);

			const void* getRaw() const;
			size_t getSize() const;

		private:
			std::vector<std::uint8_t> mData;
		};

		class VertexBuffer {
		public:
			VertexBuffer();
			VertexBuffer(const void* sourceData, size_t numBytes, eBufferUsage usage);
			~VertexBuffer();

			VertexBuffer(const VertexBuffer& ) = delete;
			void operator = (const VertexBuffer&) = delete;

			GLuint getHandle() const;

			void setData(const void* sourceData, size_t numBytes, eBufferUsage usage);
			void setSubData(size_t offset, size_t numBytes, const void* rawData);
			void getSubData(size_t offset, size_t numBytes, void* result);

		private:
			GLuint mHandle;
		};

		// -------- implementation ----------
		template <typename T>
		void VertexData::add(const T& value) {
			// [TODO] this could be optimized more

			std::uint8_t* bytes = reinterpret_cast<std::uint8_t*>(&value);

			std::copy(
				bytes,
				bytes + sizeof(value),
				std::back_inserter(mData)
			);
		}
	}
}

#endif
