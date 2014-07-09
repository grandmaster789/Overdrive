#include "render/shaderprogram.h"
#include "core/logger.h"

#include <boost/filesystem.hpp>

#include <cassert>
#include <fstream>

namespace overdrive {
	namespace render {
		ShaderProgram::ShaderProgram():
			mIsLinked(false),
			mProgramHandle(0)
		{
			std::uninitialized_fill(
				std::begin(mShaderHandles), 
				std::end(mShaderHandles), 
				0
			);
		}

		ShaderProgram::~ShaderProgram() {
			if (mProgramHandle) {
				for (auto handle : mShaderHandles) {
					if (handle != 0) {
						glDetachShader(mProgramHandle, handle);
						glDeleteShader(handle);
					}
				}

				glDeleteProgram(mProgramHandle);
			}
		}

		ShaderProgram::ShaderProgram(ShaderProgram&& sp):
			mIsLinked(sp.mIsLinked),
			mProgramHandle(sp.mProgramHandle)
		{
			std::move(
				sp.mShaderHandles.begin(), 
				sp.mShaderHandles.end(), 
				mShaderHandles.begin()
			);

			// ensure that the original doesn't do the cleanup for these anymore
			sp.mIsLinked = false;
			sp.mProgramHandle = 0;
			std::fill(
				sp.mShaderHandles.begin(), 
				sp.mShaderHandles.end(), 
				0
			);
		}

		ShaderProgram& ShaderProgram::operator = (ShaderProgram&& sp) {
			mIsLinked = sp.mIsLinked;
			mProgramHandle = sp.mProgramHandle;
			std::move(
				sp.mShaderHandles.begin(),
				sp.mShaderHandles.end(),
				mShaderHandles.begin()
			);

			// ensure that the original doesn't do the cleanup for these anymore
			sp.mIsLinked = false;
			sp.mProgramHandle = 0;
			std::fill(
				sp.mShaderHandles.begin(),
				sp.mShaderHandles.end(),
				0
			);

			return *this;
		}

		bool ShaderProgram::compileFile(const std::string& filename, eShader type) {
			if (!exists(filename)) {
				gLog << "File does not exist: " << filename;
				return false;
			}

			std::ifstream in(filename);
			if (!in.good()) {
				gLog << "Error while reading from file: " << filename;
				// TODO - more extensive error reporting
				return false;
			}

			std::ostringstream sstr;
			std::string line;

			while (std::getline(in, line))
				sstr << line << "\n";
			
			in.close();

			return compileString(sstr.str(), type);
		}

		bool ShaderProgram::compileString(const std::string& source, eShader type) {
			// lazy initialization of the program handle
			if (mProgramHandle == 0) {
				mProgramHandle = glCreateProgram();

				if (mProgramHandle == 0) {
					gLog << "Failed to create shader program";
					return false;
				}
			}

			// check for and cleanup duplicate types, if any
			if (mShaderHandles[type] != 0) {
				gLog.warning() << "Replacing shader " << type;
				
				glDetachShader(mProgramHandle, mShaderHandles[type]);
				glDeleteShader(mShaderHandles[type]);

				mShaderHandles[type] = 0;
			}

			GLuint shader = 0;
			switch (type) {
			case eShader::VERTEX:
				shader = glCreateShader(GL_VERTEX_SHADER);
				break;

			case eShader::FRAGMENT:
				shader = glCreateShader(GL_FRAGMENT_SHADER);
				break;

			case eShader::GEOMETRY:
				shader = glCreateShader(GL_GEOMETRY_SHADER);
				break;

			case eShader::TESSELATION_CONTROL:
				shader = glCreateShader(GL_TESS_CONTROL_SHADER);
				break;

			case eShader::TESSELATION_EVALUATION:
				shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
				break;

			default:
				gLog << "Unsupported shader type: " << type;
				return false;
			}

			mShaderHandles[type] = shader;

			const char* src = source.c_str();
			glShaderSource(shader, 1, &src, nullptr);

			glCompileShader(shader);

			// check for compilation errors

			GLint result;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
			
			if (result == GL_FALSE) {
				GLint messageLength = 0;

				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &messageLength);

				if (messageLength > 0) {
					std::unique_ptr<GLchar[]> message(new GLchar[messageLength]);
					GLsizei bytesWritten = 0;

					glGetShaderInfoLog(shader, messageLength, &bytesWritten, message.get());

					gLog.error() << "Shader compilation error: " << message.get();
				}

				return false;
			}
			else {
				// we're ok, attach the shader to the program
				glAttachShader(mProgramHandle, shader);
				return true;
			}
		}

		bool ShaderProgram::link() {
			if (mIsLinked) {
				gLog.warning() << "Already linked";
				return false;
			}

			if (mProgramHandle == 0) {
				gLog.error() << "No program was initialized";
				return false;
			}

			glLinkProgram(mProgramHandle);

			// check for linking errors

			GLint linkStatus = 0;

			glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &linkStatus);

			if (linkStatus == GL_FALSE) {
				GLint messageLength = 0;

				glGetProgramiv(mProgramHandle, GL_INFO_LOG_LENGTH, &messageLength);

				if (messageLength > 0) {
					std::unique_ptr<GLchar[]> message(new GLchar[messageLength]);

					GLsizei bytesWritten = 0;

					glGetProgramInfoLog(mProgramHandle, messageLength, &bytesWritten, message.get());

					gLog.error() << "Program linking error: " << message.get();
				}

				return false;
			}
			else {
				mIsLinked = true;

				gatherActiveAttributes();
				gatherActiveUniforms();

				return true;
			}
		}

		void ShaderProgram::gatherActiveAttributes() {
			mAttributeLocations.clear();

			GLint numAttributes = 0;
			GLint maxAttributeNameLength = 0;
			GLint location = 0;
			GLenum type;
			GLint size = 0;
			GLint bytesWritten = 0;

			glGetProgramiv(mProgramHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);
			glGetProgramiv(mProgramHandle, GL_ACTIVE_ATTRIBUTES, &numAttributes);

			std::unique_ptr<GLchar[]> attributeName(new GLchar[maxAttributeNameLength]);

			for (GLint i = 0; i < numAttributes; ++i) {
				glGetActiveAttrib(
					mProgramHandle,
					i,
					maxAttributeNameLength,
					&bytesWritten,
					&size,
					&type,
					attributeName.get()
					);

				location = glGetAttribLocation(mProgramHandle, attributeName.get());

				mAttributeLocations[attributeName.get()] = location;
			}
		}

		void ShaderProgram::gatherActiveUniforms() {
			mUniformLocations.clear();

			GLint numUniforms = 0;
			GLint maxUniformNameLength = 0;
			GLint location = 0;
			GLenum type;
			GLint size = 0;
			GLint bytesWritten = 0;

			glGetProgramiv(mProgramHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
			glGetProgramiv(mProgramHandle, GL_ACTIVE_UNIFORMS, &numUniforms);

			std::unique_ptr<GLchar[]> uniformName(new GLchar[maxUniformNameLength]);

			for (GLint i = 0; i < numUniforms; ++i) {
				glGetActiveUniform(
					mProgramHandle,
					i,
					maxUniformNameLength,
					&bytesWritten,
					&size,
					&type,
					uniformName.get()
				);

				location = glGetUniformLocation(mProgramHandle, uniformName.get());

				mUniformLocations[uniformName.get()] = location;
			}
		}

		bool ShaderProgram::validate() const {
			if (!mIsLinked) {
				gLog << "Cannot validate unlinked program";
				return false;
			}

			GLint status = 0;
			
			glValidateProgram(mProgramHandle);

			glGetProgramiv(mProgramHandle, GL_VALIDATE_STATUS, &status);

			if (status == GL_FALSE) {
				GLint messageLength = 0;

				glGetProgramiv(mProgramHandle, GL_INFO_LOG_LENGTH, &messageLength);

				if (messageLength > 0) {
					std::unique_ptr<GLchar[]> message(new GLchar[messageLength]);
					
					GLsizei bytesWritten = 0;

					glGetProgramInfoLog(mProgramHandle, messageLength, &bytesWritten, message.get());

					gLog << "Validation error: " << message.get();
				}

				return false;
			}

			gLog << "Program validated successfully";

			return true;
		}

		void ShaderProgram::use() const {
			assert(mProgramHandle != 0);
			assert(mIsLinked);

			glUseProgram(mProgramHandle);
		}

		GLuint ShaderProgram::getHandle() const {
			return mProgramHandle;
		}

		bool ShaderProgram::isLinked() const {
			return mIsLinked;
		}

		/*
		void ShaderProgram::bindAttribute(GLuint location, const std::string& name) const {
			assert(mProgramHandle != 0);
			glBindAttribLocation(mProgramHandle, location, name.c_str());
		}

		void ShaderProgram::bindFragData(GLuint location, const std::string& name) const {
			assert(mProgramHandle != 0);
			glBindFragDataLocation(mProgramHandle, location, name.c_str());
		}
		*/

		void ShaderProgram::set(const std::string& name, float x, float y, float z) const {
			glUniform3f(getUniformLocation(name), x, y, z);
		}

		void ShaderProgram::set(const std::string& name, const glm::vec2& v) const {
			glUniform2f(getUniformLocation(name), v.x, v.y);
		}

		void ShaderProgram::set(const std::string& name, const glm::vec3& v) const {
			glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
		}
		
		void ShaderProgram::set(const std::string& name, const glm::vec4& v) const {
			glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
		}

		void ShaderProgram::set(const std::string& name, const glm::mat3& m) const {
			glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::set(const std::string& name, const glm::mat4& m) const {
			glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::set(const std::string& name, float f) const {
			glUniform1f(getUniformLocation(name), f);
		}

		void ShaderProgram::set(const std::string& name, int i) const {
			glUniform1i(getUniformLocation(name), i);
		}

		void ShaderProgram::set(const std::string& name, bool b) const {
			if (b)
				glUniform1i(getUniformLocation(name), 1);
			else
				glUniform1i(getUniformLocation(name), 0);
		}

		void ShaderProgram::listActiveUniforms() const {
			GLint numUniforms = 0;
			GLint maxUniformNameLength = 0;
			GLint location = 0;
			GLenum type;
			GLint size = 0;
			GLint bytesWritten = 0;

			glGetProgramiv(mProgramHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
			glGetProgramiv(mProgramHandle, GL_ACTIVE_UNIFORMS, &numUniforms);

			std::unique_ptr<GLchar[]> uniformName(new GLchar[maxUniformNameLength]);

			gLog << " ---- Active Uniforms ---- ";
			
			for (GLint i = 0; i < numUniforms; ++i) {
				glGetActiveUniform(
					mProgramHandle,
					i,
					maxUniformNameLength,
					&bytesWritten,
					&size,
					&type,
					uniformName.get()
				);

				location = glGetUniformLocation(mProgramHandle, uniformName.get());

				//TODO: add type information
				gLog << i << ":\t" << uniformName.get() << " => " << location;
			}

			gLog << " ------------------------- ";
		}

		void ShaderProgram::listActiveAttributes() const {
			GLint numAttributes = 0;
			GLint maxAttributeNameLength = 0;
			GLint location = 0;
			GLenum type;
			GLint size = 0;
			GLint bytesWritten = 0;

			glGetProgramiv(mProgramHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);
			glGetProgramiv(mProgramHandle, GL_ACTIVE_ATTRIBUTES, &numAttributes);

			std::unique_ptr<GLchar[]> attributeName(new GLchar[maxAttributeNameLength]);

			gLog << " ---- Active Attributes ---- ";

			for (GLint i = 0; i < numAttributes; ++i) {
				glGetActiveAttrib(
					mProgramHandle,
					i,
					maxAttributeNameLength,
					&bytesWritten,
					&size,
					&type,
					attributeName.get()
				);

				location = glGetAttribLocation(mProgramHandle, attributeName.get());

				//TODO: add type information
				gLog << i << ":\t" << attributeName.get() << " => " << location;
			}

			gLog << " ------------------------- ";
		}
		
		bool ShaderProgram::exists(const std::string& filename) const {
			boost::filesystem::path p(filename);

			if (boost::filesystem::exists(p) &&
				boost::filesystem::is_regular_file(p))
				return true;

			return false;
		}
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::render::eShader& shaderType) {
	switch (shaderType) {
	case overdrive::render::eShader::VERTEX:
		os << "vertex";
		break;

	case overdrive::render::eShader::FRAGMENT:
		os << "fragment";
		break;

	case overdrive::render::eShader::GEOMETRY:
		os << "geometry";
		break;

	case overdrive::render::eShader::TESSELATION_CONTROL:
		os << "tesselation control";
		break;

	case overdrive::render::eShader::TESSELATION_EVALUATION:
		os << "tesselation evaluation";
		break;

	default:
		os << "unknown";
	}

	return os;
}