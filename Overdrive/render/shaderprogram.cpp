#include "stdafx.h"
#include "shaderprogram.h"
#include "../core/logger.h"
#include <fstream>

namespace overdrive {
	namespace render {
		namespace {
			int getShaderIndex(eShaderType type) {
				switch (type) {
				case eShaderType::VERTEX: return 0;
				case eShaderType::FRAGMENT: return 1;
				case eShaderType::GEOMETRY: return 2;
				case eShaderType::TESSELATION_CONTROL: return 3;
				case eShaderType::TESSELATION_EVAL: return 4;
				case eShaderType::COMPUTE: return 5;
				default:
					throw ShaderException("Unsupported shader type");
				}
			}
		}

		ShaderProgram::ShaderProgram():
			mHandle(0),
			mIsLinked(false)
		{
		}

		ShaderProgram::~ShaderProgram() {
			if (mHandle) {
				for (const auto& shader : mShaders)
					if (shader)
						glDetachShader(mHandle, shader->getHandle());

				glDeleteProgram(mHandle);
			}
		}

		void ShaderProgram::attachShader(const std::string& source, eShaderType type) {
			if (mHandle == 0) {
				mHandle = glCreateProgram();

				if (mHandle == 0) {
					gLogError << "Failed to create shader program";
					throw ShaderException("Failed to create shader program");
				}
			}

			int idx = getShaderIndex(type);

			if (mShaders[idx]) {
				gLogWarning << "Duplicate shader type provided; overwriting previous shader";
				glDetachShader(mHandle, mShaders[idx]->getHandle());
			}

			mShaders[idx] = std::make_unique<Shader>(type);
			mShaders[idx]->compile(source);
			glAttachShader(mHandle, mShaders[idx]->getHandle());
		}

		void ShaderProgram::loadShader(const boost::filesystem::path& p, eShaderType type) {
			using namespace boost::filesystem;

			if (!exists(p)) {
				gLogError << "File not found: " << p;
				throw ShaderException("File not found");
			}

			if (!is_regular_file(p)) {
				gLogError << "Not a regular file: " << p;
				throw ShaderException("Not a regular file");
			}

			std::ifstream ifs(p.c_str());
			if (!ifs.good()) {
				gLogError << "Could not open file: " << p;
				throw ShaderException("Could not open file");
			}

			std::string content(
				(std::istreambuf_iterator<char>(ifs)),  // <~ Most Vexing Parse... hence the extra ()
				std::istreambuf_iterator<char>()
			);

			attachShader(content, type);
		}

		void ShaderProgram::link() {
			if (mIsLinked) {
				gLogWarning << "Shader program already linked";
				return;
			}

			if (mHandle == 0) {
				gLogError << "No program handle available, cannot link";
				return;
			}

			glLinkProgram(mHandle);

			GLint result;
			glGetProgramiv(mHandle, GL_LINK_STATUS, &result);

			if (result == GL_FALSE) {
				// something went wrong, extract and log the error message
				GLint messageLength = 0;

				glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &messageLength);

				if (messageLength > 0) {
					auto message = std::make_unique<char[]>(messageLength);
					GLsizei bytesWritten = 0;

					glGetProgramInfoLog(
						mHandle,
						messageLength,
						&bytesWritten,
						message.get()
					);

					gLogError << message.get();
				}
			}

			mIsLinked = true;
		}

		bool ShaderProgram::isLinked() const {
			return mIsLinked;
		}

		void ShaderProgram::validate() {
		}

		void ShaderProgram::bind() {
			assert(mHandle);
			glUseProgram(mHandle);
		}

		void ShaderProgram::unbind() {
			glUseProgram(0);
		}

		void ShaderProgram::listUniforms() {
		}

		void ShaderProgram::listAttributes() {
		}

		Shader* ShaderProgram::getShader(eShaderType type) const {
			return mShaders[getShaderIndex(type)].get();
		}
	}
}
