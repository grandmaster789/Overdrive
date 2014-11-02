#include "render/shader.h"
#include "core/logger.h"
#include <memory>

namespace overdrive {
	namespace render {
		Shader::Shader(eShaderType type) {
			switch (type) {
			case eShaderType::VERTEX:
				mShaderHandle = glCreateShader(GL_VERTEX_SHADER);
				break;

			case eShaderType::FRAGMENT:
				mShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
				break;

			case eShaderType::GEOMETRY:
				mShaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
				break;

			case eShaderType::TESSELATION_CONTROL:
				mShaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
				break;

			case eShaderType::TESSELATION_EVALUATION:
				mShaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
				break;

			case eShaderType::COMPUTE:
				mShaderHandle = glCreateShader(GL_COMPUTE_SHADER);
				break;

			default:
				throw std::runtime_error("unsupported shader type");
			}
		}

		Shader::~Shader() {
			if (mShaderHandle)
				glDeleteShader(mShaderHandle);
		}

		bool Shader::compile(std::string source) {
			const char* raw = source.c_str();

			glShaderSource(mShaderHandle, 1, &raw, nullptr);
			glCompileShader(mShaderHandle);

			GLint result;
			glGetShaderiv(mShaderHandle, GL_COMPILE_STATUS, &result);

			if (result == GL_FALSE) {
				// something went wrong, extract and log the error message
				GLint messageLength = 0;

				glGetShaderiv(mShaderHandle, GL_INFO_LOG_LENGTH, &messageLength);

				if (messageLength > 0) {
					auto message = std::make_unique<char[]>(messageLength);
					GLsizei bytesWritten = 0;
					
					glGetShaderInfoLog(
						mShaderHandle, 
						messageLength, 
						&bytesWritten,
						message.get()
					);

					gLog.error() << message.get();
				}

				return false;
			}

			return true;
		}

		GLuint Shader::getHandle() const {
			return mShaderHandle;
		}
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::render::eShaderType& type) {
	switch (type) {
	case overdrive::render::eShaderType::VERTEX:
		os << "Vertex Shader";
		break;

	case overdrive::render::eShaderType::FRAGMENT:
		os << "Fragment Shader";
		break;

	case overdrive::render::eShaderType::GEOMETRY:
		os << "Geometry Shader";
		break;

	case overdrive::render::eShaderType::TESSELATION_EVALUATION:
		os << "Tesselation Evaluation Shader";
		break;

	case overdrive::render::eShaderType::TESSELATION_CONTROL:
		os << "Tesselation Control Shader";
		break;

	case overdrive::render::eShaderType::COMPUTE:
		os << "Compute Shader";
		break;

	default:
		os << "Unknown shader type: " << static_cast<int>(type);
		break;
	}

	return os;
}