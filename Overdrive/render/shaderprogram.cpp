#include "render/shaderprogram.h"
#include "core/logger.h"
#include "util/checkGL.h"
#include <fstream>
#include <boost/filesystem.hpp>

namespace overdrive {
	namespace render {
		ShaderProgram::ShaderProgram() :
			mProgramHandle(0),
			mIsLinked(false)
		{
		}

		ShaderProgram::~ShaderProgram() {
			if (mProgramHandle) {
				for (auto& shader : mProgramShaders)
					if (shader)
						glDetachShader(mProgramHandle, shader->getHandle());

				glDeleteProgram(mProgramHandle);
			}
		}

		bool ShaderProgram::addShaderFile(std::string filename, eShaderType type) {
			boost::filesystem::path p(filename);

			if (!boost::filesystem::exists(p)) {
				gLog << "File not found: [" << filename << "]";
				return false;
			}

			if (!boost::filesystem::is_regular_file(p)) {
				gLog << "Path specified is not a regular file: " << filename;
				return false;
			}

			std::ifstream input(filename);
			if (!input.good()) {
				gLog << "Could not open file: [" << filename << "]";
				return false;
			}

			std::ostringstream sstr;
			std::string line;

			while (std::getline(input, line))
				sstr << line << "\n";

			return addShaderSource(sstr.str(), type);
		}

		bool ShaderProgram::addShaderSource(std::string source, eShaderType type) {
			// lazy initialization of the program handle
			if (mProgramHandle == 0) {
				mProgramHandle = glCreateProgram();

				if (mProgramHandle == 0) {
					gLog.error() << "Failed to create shader program";
					return false;
				}
			}

			if (mProgramShaders[type]) {
				gLog.warning() << "Duplicate shader type provided; overwriting previous shader (" << type << ")";

				glDetachShader(mProgramHandle, mProgramShaders[type]->getHandle());
			}

			mProgramShaders[type] = std::make_unique<Shader>(type);
			
			if (mProgramShaders[type]->compile(std::move(source)))
				glAttachShader(mProgramHandle, mProgramShaders[type]->getHandle());

			return CHECK_GL_STATE;
		}

		bool ShaderProgram::link() {
			if (mIsLinked) {
				gLog.warning() << "Shader program already linked";
				return true;
			}

			if (mProgramHandle == 0) {
				gLog.error() << "No program handle available, cannot link";
				return false;
			}

			glLinkProgram(mProgramHandle);

			GLint result;
			glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &result);
			
			if (result == GL_FALSE) {
				// something went wrong, extract and log the error message
				GLint messageLength = 0;

				glGetProgramiv(mProgramHandle, GL_INFO_LOG_LENGTH, &messageLength);

				if (messageLength > 0) {
					auto message = std::make_unique<char[]>(messageLength);
					GLsizei bytesWritten = 0;

					glGetProgramInfoLog(
						mProgramHandle,
						messageLength,
						&bytesWritten,
						message.get()
					);

					gLog.error() << message.get();
				}

				return false;
			}
			
			CHECK_GL_STATE;

			gatherAttributes();
			gatherUniforms();

			mIsLinked = true;

			return true;
		}

		bool ShaderProgram::validate() {
			if (!isLinked()) {
				gLog.warning() << "Cannot validate a shader program that is not linked yet";
				return false;
			}

			glValidateProgram(mProgramHandle);

			GLint result;
			glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &result);

			if (result == GL_FALSE) {
				// something went wrong, extract and log the error message
				GLint messageLength = 0;

				glGetProgramiv(
					mProgramHandle, 
					GL_INFO_LOG_LENGTH, 
					&messageLength
				);

				if (messageLength > 0) {
					auto message = std::make_unique<char[]>(messageLength);
					GLsizei bytesWritten = 0;

					glGetProgramInfoLog(
						mProgramHandle,
						messageLength,
						&bytesWritten,
						message.get()
					);

					gLog.error() << message.get();
				}

				return false;
			}

			CHECK_GL_STATE;

			return true;
		}

		void ShaderProgram::use() const {
			assert(isLinked());
			glUseProgram(mProgramHandle);
		}

		bool ShaderProgram::isLinked() const {
			return mIsLinked;
		}

		std::string uniformTypeString(GLenum value) {
			switch (value) {
			case GL_FLOAT: return "GL_FLOAT"; break;
			case GL_FLOAT_VEC2: return "GL_FLOAT_VEC2"; break;
			case GL_FLOAT_VEC3: return "GL_FLOAT_VEC3"; break;
			case GL_FLOAT_VEC4: return "GL_FLOAT_VEC4"; break;
			case GL_INT: return "GL_INT"; break;
			case GL_INT_VEC2: return "GL_INT_VEC2"; break;
			case GL_INT_VEC3: return "GL_INT_VEC3"; break;
			case GL_INT_VEC4: return "GL_INT_VEC4"; break;
			case GL_BOOL: return "GL_BOOL"; break;
			case GL_BOOL_VEC2: return "GL_BOOL_VEC2"; break;
			case GL_BOOL_VEC3: return "GL_BOOL_VEC3"; break;
			case GL_BOOL_VEC4: return "GL_BOOL_VEC4"; break;
			case GL_FLOAT_MAT2: return "GL_FLOAT_MAT2"; break;
			case GL_FLOAT_MAT3: return "GL_FLOAT_MAT3"; break;
			case GL_FLOAT_MAT4: return "GL_FLOAT_MAT4"; break;
			case GL_SAMPLER_2D: return "GL_SAMPLER_2D"; break;
			case GL_SAMPLER_CUBE: return "GL_SAMPLER_CUBE";break;
			default: return "Unknown";
			}
		}

		void ShaderProgram::printUniforms() {
			GLint numUniforms = 0;
			GLint maxUniformNameLength = 0;
			GLint location = 0;
			GLenum type;
			GLint size = 0;
			GLint bytesWritten = 0;

			glGetProgramiv(mProgramHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
			glGetProgramiv(mProgramHandle, GL_ACTIVE_UNIFORMS, &numUniforms);

			std::unique_ptr<GLchar[]> uniformName(new GLchar[maxUniformNameLength]);

			gLog.debug() << " ---- Active Uniforms ---- ";

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

				gLog << i << 
					":\t" << uniformName.get() << 
					" (" << uniformTypeString(type) << ", " << size << ") " <<
					"=> " << location;
			}

			gLog.debug() << " ------------------------- ";
		}

		std::string attributeTypeString(GLenum value) {
			switch (value) {
			case GL_FLOAT: return "GL_FLOAT"; break;
			case GL_FLOAT_VEC2: return "GL_FLOAT_VEC2"; break;
			case GL_FLOAT_VEC3: return "GL_FLOAT_VEC3"; break;
			case GL_FLOAT_VEC4: return "GL_FLOAT_VEC4"; break;
			case GL_FLOAT_MAT2: return "GL_FLOAT_MAT2"; break;
			case GL_FLOAT_MAT3: return "GL_FLOAT_MAT3"; break;
			case GL_FLOAT_MAT4: return "GL_FLOAT_MAT4"; break;
			default: return "Unknown";
			}
		}

		void ShaderProgram::printAttributes() {
			GLint numAttributes = 0;
			GLint maxAttributeNameLength = 0;
			GLint location = 0;
			GLenum type;
			GLint size = 0;
			GLint bytesWritten = 0;

			glGetProgramiv(mProgramHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);
			glGetProgramiv(mProgramHandle, GL_ACTIVE_ATTRIBUTES, &numAttributes);

			std::unique_ptr<GLchar[]> attributeName(new GLchar[maxAttributeNameLength]);

			gLog.debug() << " ---- Active Attributes ---- ";

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

				gLog << i <<
					":\t" << attributeName.get() <<
					" (" << attributeTypeString(type) << ", " << size << ") " <<
					"=> " << location;
			}

			gLog.debug() << " ------------------------- ";
		}

		void ShaderProgram::set(const std::string& name, float x, float y, float z) const {
			glUniform3f(getUniformLocation(std::move(name)), x, y, z);
		}

		void ShaderProgram::set(const std::string& name, const glm::vec2& v) const {
			glUniform2f(getUniformLocation(std::move(name)), v.x, v.y);
		}

		void ShaderProgram::set(const std::string& name, const glm::vec3& v) const {
			glUniform3f(getUniformLocation(std::move(name)), v.x, v.y, v.z);
		}

		void ShaderProgram::set(const std::string& name, const glm::vec4& v) const {
			glUniform4f(getUniformLocation(std::move(name)), v.x, v.y, v.z, v.w);
		}

		void ShaderProgram::set(const std::string& name, const glm::mat3& m) const {
			glUniformMatrix3fv(getUniformLocation(std::move(name)), 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::set(const std::string& name, const glm::mat4& m) const {
			glUniformMatrix4fv(getUniformLocation(std::move(name)), 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::set(const std::string& name, float f) const {
			glUniform1f(getUniformLocation(std::move(name)), f);
		}

		void ShaderProgram::set(const std::string& name, int i) const {
			glUniform1i(getUniformLocation(std::move(name)), i);
		}

		void ShaderProgram::set(const std::string& name, bool b) const {
			if (b)
				glUniform1i(getUniformLocation(name), 1);
			else
				glUniform1i(getUniformLocation(name), 0);
		}

		void ShaderProgram::gatherUniforms() {
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

		void ShaderProgram::gatherAttributes() {
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
	}
}
