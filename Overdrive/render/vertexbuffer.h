#ifndef OVERDRIVE_RENDER_VERTEXBUFFER_H
#define OVERDRIVE_RENDER_VERTEXBUFFER_H

#include "opengl.h"
#include "render/enumerations.h"
#include "util/checkGL.h"
#include <vector>

namespace overdrive {
	namespace render {
		template <typename T, int tNumComponents = 1>
		class VertexBuffer {
		public:
			VertexBuffer(eBufferUsage usage = eBufferUsage::STATIC_DRAW);
			~VertexBuffer();

			VertexBuffer(const VertexBuffer&) = delete;
			VertexBuffer& operator=  (const VertexBuffer&) = delete;

			GLuint getHandle() const;
			size_t getNumBytes() const;
			size_t getNumElements() const;
			
			void setData(const T* data, size_t numBytes);
			void setData(const std::vector<T>& data);

		private:
			GLuint mHandle;
			size_t mNumBytes;
			eBufferUsage mBufferUsage;
		};

		// convenience functions for filling a buffer
		// [Note] - could also be written with variadic templates, at the cost of readability
		template <typename T> void push(std::vector<T>& buffer, const T& x);
		template <typename T> void push(std::vector<T>& buffer, const T& x, const T& y);
		template <typename T> void push(std::vector<T>& buffer, const T& x, const T& y, const T& z);

		// convenience function for adding glm structs
		void append(std::vector<glm::vec2::value_type>& buffer, const glm::vec2& v);
		void append(std::vector<glm::vec3::value_type>& buffer, const glm::vec3& v);
		void append(std::vector<glm::vec4::value_type>& buffer, const glm::vec4& v);
	}
}

#include "render/vertexbuffer.inl"

#endif
