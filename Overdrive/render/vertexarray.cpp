#include "render/vertexarray.h"

namespace overdrive {
	namespace render {
		VertexArray::VertexArray():
			mHandle(0)
		{
			glGenVertexArrays(1, &mHandle);
		}

		VertexArray::~VertexArray() {
			if (mHandle)
				glDeleteVertexArrays(1, &mHandle);
		}

		GLuint VertexArray::getHandle() const {
			return mHandle;
		}

		void VertexArray::bindElements(const VertexBuffer<GLuint, 1>& indexBuffer) {
			glBindVertexArray(mHandle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.getHandle());
		}

		void append(std::vector<glm::vec2::value_type>& buffer, const glm::vec2& v) {
			buffer.push_back(v.x);
			buffer.push_back(v.y);
		}

		void append(std::vector<glm::vec3::value_type>& buffer, const glm::vec3& v) {
			buffer.push_back(v.x);
			buffer.push_back(v.y);
			buffer.push_back(v.z);
		}

		void append(std::vector<glm::vec4::value_type>& buffer, const glm::vec4& v) {
			buffer.push_back(v.x);
			buffer.push_back(v.y);
			buffer.push_back(v.z);
			buffer.push_back(v.w);
		}
	}
}
