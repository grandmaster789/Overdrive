#include "stdafx.h"
#include "shader.h"
#include "../core/logger.h"
#include <memory>

namespace overdrive {
	namespace render {
		ShaderException::ShaderException(const std::string& description) :
			std::runtime_error(description)
		{
		}

		Shader::Shader(eShaderType type):
			mType(type),
			mHandle(0)
		{
			mHandle = glCreateShader(static_cast<GLenum>(type));
			if (!mHandle)
				throw ShaderException("Failed to create shader");
		}

		Shader::~Shader() {
			if (mHandle)
				glDeleteShader(mHandle);
		}

		Shader::Shader(Shader&& s) :
			mType(s.mType),
			mHandle(s.mHandle)
		{
			s.mHandle = 0;
		}

		Shader& Shader::operator = (Shader&& s) {
			if (mHandle)
				glDeleteShader(mHandle);

			mType = s.mType;
			mHandle = s.mHandle;

			s.mHandle = 0;
			
			return *this;
		}

		GLuint Shader::getHandle() const {
			return mHandle;
		}

		void Shader::compile(const std::string& source) {
			const char* raw = source.c_str();

			glShaderSource(mHandle, 1, &raw, nullptr);
			glCompileShader(mHandle);

			GLint result;
			glGetShaderiv(mHandle, GL_COMPILE_STATUS, &result);

			if (result == GL_FALSE) {
				// something went wrong, extract and log the error message
				GLint messageLength = 0;

				glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &messageLength);

				if (messageLength > 0) {
					auto message = std::make_unique<char[]>(messageLength);
					GLsizei bytesWritten = 0;

					glGetShaderInfoLog(
						mHandle,
						messageLength,
						&bytesWritten,
						message.get()
					);

					gLogError << message.get();
					
					throw ShaderException(message.get());
				}
			}
		}

		std::string Shader::getSource() const {
			assert(mHandle);

			GLsizei numBytes = 0;
			glGetShaderiv(mHandle, GL_SHADER_SOURCE_LENGTH, &numBytes);

			GLsizei bytesWritten = 0;

			auto buffer = std::make_unique<GLchar[]>(numBytes);
			glGetShaderSource(mHandle, numBytes, &bytesWritten, buffer.get());

			return buffer.get(); // implicit conversion to std::string
		}

		std::ostream& operator << (std::ostream& os, const eShaderType& type) {
			switch (type) {
			case eShaderType::VERTEX: os << "Vertex Shader"; break;
			case eShaderType::FRAGMENT: os << "Fragment Shader"; break;
			case eShaderType::GEOMETRY: os << "Geometry Shader"; break;
			case eShaderType::TESSELATION_CONTROL: os << "Tesselation control Shader"; break;
			case eShaderType::TESSELATION_EVAL: os << "Tesselation evaluation Shader"; break;
			case eShaderType::COMPUTE: os << "Compute Shader"; break;
			default:
				os << "Unknown shader type: " << static_cast<std::underlying_type<eShaderType>::type>(type);
			}

			return os;
		}
	}
}