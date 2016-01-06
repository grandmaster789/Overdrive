#include "stdafx.h"
#include "shaderprogram.h"
#include "shaderAttribute.h"
#include "shaderUniform.h"
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

		GLuint ShaderProgram::getHandle() const {
			return mHandle;
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
					return;
				}
			}

			mIsLinked = true;

#ifdef OVERDRIVE_DEBUG
			validate();
#endif

			gatherUniforms();
			gatherAttributes();
		}

		bool ShaderProgram::isLinked() const {
			return mIsLinked;
		}

		void ShaderProgram::validate() {
			if (!isLinked()) {
				gLogWarning << "Cannot validate a shader program that is not linked yet";
				return;
			}

			glValidateProgram(mHandle);

			GLint result;
			glGetProgramiv(mHandle, GL_LINK_STATUS, &result);

			if (result == GL_FALSE) {
				// something went wrong, extract and log the error message
				GLint messageLength = 0;

				glGetProgramiv(
					mHandle,
					GL_INFO_LOG_LENGTH,
					&messageLength
				);

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
		}

		void ShaderProgram::bind() {
			assert(mHandle);
			glUseProgram(mHandle);
		}

		void ShaderProgram::unbind() {
			glUseProgram(0);
		}

		void ShaderProgram::listUniforms() const {
			// [NOTE] perhaps sort the names alphabetically?

			gLogDebug << " ---- Active Uniforms ---- ";
			gLogDebug << "Number of uniforms: " << mUniforms.size();

			unsigned int i = 0;

			for (const auto& item : mUniforms)
				gLog << i++ << ":\t" << item.first << " " << item.second;

			gLogDebug << " ------------------------- ";
		}

		void ShaderProgram::listAttributes() const {
			// [NOTE] perhapse sort the attributes alphabetically?

			gLogDebug << " ---- Active Attributes ---- ";
			gLogDebug << " Number of attributes: " << mAttributes.size();

			unsigned int i = 0;

			for (const auto& item : mAttributes)
				gLog << i++ << ":\t" << item.second;

			gLogDebug << " ------------------------- ";
		}

		Shader* ShaderProgram::getShader(eShaderType type) const {
			return mShaders[getShaderIndex(type)].get();
		}

		/*
		void ShaderProgram::bindAttributeLocation(GLuint id, const std::string& name) {
		}

		void ShaderProgram::bindFragDataLocation(GLuint id, const std::string& name) {
		}
		*/

		GLint ShaderProgram::getUniformLocation(const std::string& name) const {
			auto it = mUniforms.find(name);

			if (it == mUniforms.end())
				throw ShaderException(std::string("Cannot locate uniform: ") + name);

			return it->second.mLocation;
		}

		const ShaderUniform& ShaderProgram::getUniformData(const std::string& name) const {
			auto it = mUniforms.find(name);

			if (it == mUniforms.end())
				throw ShaderException(std::string("Cannot locate uniform: ") + name);

			return it->second;
		}

		GLint ShaderProgram::getAttributeLocation(const std::string& name) const {
			auto it = mAttributes.find(name);
			
			if (it == mAttributes.end())
				throw ShaderException(std::string("Cannot locate attribute: ") + name);

			return it->second.getLocation();
		}

		const ShaderAttribute& ShaderProgram::getAttributeData(const std::string& name) const {
			auto it = mAttributes.find(name);

			if (it == mAttributes.end())
				throw ShaderException(std::string("Cannot locate attribute: ") + name);

			return it->second;
		}

		// http://docs.gl/gl4/glGetProgramInterface
		// http://docs.gl/gl4/glGetProgramResource
		void ShaderProgram::gatherUniforms() {
			using std::pair;
			using std::string;

			typedef pair<string, ShaderUniform> UniformPair;

			GLint numUniforms = 0;
			
			glGetProgramInterfaceiv(mHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

			// [NOTE] this must correspond to the constructor in ShaderUniform!
			GLenum properties[] = {
				GL_NAME_LENGTH,
				GL_TYPE,
				GL_LOCATION,

				GL_ARRAY_SIZE,
				GL_ARRAY_STRIDE,
				GL_MATRIX_STRIDE,

				GL_OFFSET,
				GL_BLOCK_INDEX,
				GL_ATOMIC_COUNTER_BUFFER_INDEX,

				GL_IS_ROW_MAJOR,

				GL_REFERENCED_BY_VERTEX_SHADER,
				GL_REFERENCED_BY_FRAGMENT_SHADER,
				GL_REFERENCED_BY_GEOMETRY_SHADER,
				GL_REFERENCED_BY_TESS_CONTROL_SHADER,
				GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
				GL_REFERENCED_BY_COMPUTE_SHADER
			};

			const size_t numProperties = sizeof(properties) / sizeof(properties[0]);

			for (GLint i = 0; i < numUniforms; ++i) {
				GLint results[numProperties];

				glGetProgramResourceiv(
					mHandle, 
					GL_UNIFORM, 
					i, 
					numProperties, 
					properties, 
					numProperties, 
					nullptr, 
					results
				);

				GLint nameLength = results[0];
				
				std::unique_ptr<GLchar[]> uniformName(new GLchar[nameLength]);

				glGetProgramResourceName(
					mHandle, 
					GL_UNIFORM, 
					i, 
					nameLength, 
					nullptr, 
					uniformName.get()
				);

				mUniforms.insert(UniformPair(
					uniformName.get(),
					ShaderUniform(uniformName.get(), results)
				));
			}
		}

		// http://docs.gl/gl4/glGetProgramInterface
		// http://docs.gl/gl4/glGetProgramResource
		void ShaderProgram::gatherAttributes() {
			using std::pair;
			using std::string;

			typedef pair<string, ShaderAttribute> AttributePair;

			GLint numAttributes = 0;
			glGetProgramInterfaceiv(mHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttributes);

			// [NOTE] this must correspond to the constructor in ShaderAttribute!
			GLenum properties[] = {
				GL_NAME_LENGTH,
				GL_TYPE,
				GL_LOCATION,

				GL_REFERENCED_BY_VERTEX_SHADER,
				GL_REFERENCED_BY_FRAGMENT_SHADER,
				GL_REFERENCED_BY_GEOMETRY_SHADER,
				GL_REFERENCED_BY_TESS_CONTROL_SHADER,
				GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
				GL_REFERENCED_BY_COMPUTE_SHADER,

				GL_IS_PER_PATCH,
				GL_LOCATION_COMPONENT
			};

			const size_t numProperties = sizeof(properties) / sizeof(properties[0]);

			for (GLint i = 0; i < numAttributes; ++i) {
				GLint results[numProperties];
				glGetProgramResourceiv(
					mHandle,
					GL_PROGRAM_INPUT,
					i,
					numProperties,
					properties,
					numProperties,
					nullptr,
					results
				);

				GLint nameSize = results[0];
				std::unique_ptr<GLchar[]> attributeName(new GLchar[nameSize]);

				glGetProgramResourceName(
					mHandle, 
					GL_PROGRAM_INPUT, 
					i, 
					nameSize, 
					nullptr, 
					attributeName.get()
				);

				mAttributes.insert(AttributePair(
					attributeName.get(),
					ShaderAttribute(attributeName.get(), results)
				));
			}
		}

		void ShaderProgram::setUniform(GLint location, GLfloat x) {
			glUniform1f(location, x);
		}

		void ShaderProgram::setUniform(GLint location, GLfloat x, GLfloat y) {
			glUniform2f(location, x, y);
		}

		void ShaderProgram::setUniform(GLint location, GLfloat x, GLfloat y, GLfloat z) {
			glUniform3f(location, x, y, z);
		}

		void ShaderProgram::setUniform(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
			glUniform4f(location, x, y, z, w);
		}

		void ShaderProgram::setUniform(GLint location, const glm::vec2& v) {
			glUniform2f(location, v.x, v.y);
		}

		void ShaderProgram::setUniform(GLint location, const glm::vec3& v) {
			glUniform3f(location, v.x, v.y, v.z);
		}

		void ShaderProgram::setUniform(GLint location, const glm::vec4& v) {
			glUniform4f(location, v.x, v.y, v.z, v.w);
		}
			 
		void ShaderProgram::setUniform(GLint location, GLdouble x) {
			glUniform1d(location, x);
		}

		void ShaderProgram::setUniform(GLint location, GLdouble x, GLdouble y) {
			glUniform2d(location, x, y);
		}

		void ShaderProgram::setUniform(GLint location, GLdouble x, GLdouble y, GLdouble z) {
			glUniform3d(location, x, y, z);
		}

		void ShaderProgram::setUniform(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
			glUniform4d(location, x, y, z, w);
		}

		void ShaderProgram::setUniform(GLint location, const glm::dvec2& v) {
			glUniform2d(location, v.x, v.y);
		}

		void ShaderProgram::setUniform(GLint location, const glm::dvec3& v) {
			glUniform3d(location, v.x, v.y, v.z);
		}

		void ShaderProgram::setUniform(GLint location, const glm::dvec4& v) {
			glUniform4d(location, v.x, v.y, v.z, v.w);
		}
			 
		void ShaderProgram::setUniform(GLint location, GLint x) {
			glUniform1i(location, x);
		}

		void ShaderProgram::setUniform(GLint location, GLint x, GLint y) {
			glUniform2i(location, x, y);
		}

		void ShaderProgram::setUniform(GLint location, GLint x, GLint y, GLint z) {
			glUniform3i(location, x, y, z);
		}

		void ShaderProgram::setUniform(GLint location, GLint x, GLint y, GLint z, GLint w) {
			glUniform4i(location, x, y, z, w);
		}

		void ShaderProgram::setUniform(GLint location, const glm::ivec2& v) {
			glUniform2i(location, v.x, v.y);
		}

		void ShaderProgram::setUniform(GLint location, const glm::ivec3& v) {
			glUniform3i(location, v.x, v.y, v.z);
		}

		void ShaderProgram::setUniform(GLint location, const glm::ivec4& v) {
			glUniform4i(location, v.x, v.y, v.z, v.w);
		}
			 
		void ShaderProgram::setUniform(GLint location, GLuint x) {
			glUniform1ui(location, x);
		}

		void ShaderProgram::setUniform(GLint location, GLuint x, GLuint y) {
			glUniform2ui(location, x, y);
		}

		void ShaderProgram::setUniform(GLint location, GLuint x, GLuint y, GLuint z) {
			glUniform3ui(location, x, y, z);
		}
		
		void ShaderProgram::setUniform(GLint location, GLuint x, GLuint y, GLuint z, GLuint w) {
			glUniform4ui(location, x, y, z, w);
		}

		void ShaderProgram::setUniform(GLint location, const glm::uvec2& v) {
			glUniform2ui(location, v.x, v.y);
		}

		void ShaderProgram::setUniform(GLint location, const glm::uvec3& v) {
			glUniform3ui(location, v.x, v.y, v.z);
		}

		void ShaderProgram::setUniform(GLint location, const glm::uvec4& v) {
			glUniform4ui(location, v.x, v.y, v.z, v.w);
		}
			 
		void ShaderProgram::setUniform(GLint location, GLboolean x) {
			glUniform1i(location, x);
		}

		void ShaderProgram::setUniform(GLint location, GLboolean x, GLboolean y) {
			glUniform2i(location, x, y);
		}

		void ShaderProgram::setUniform(GLint location, GLboolean x, GLboolean y, GLboolean z) {
			glUniform3i(location, x, y, z);
		}

		void ShaderProgram::setUniform(GLint location, GLboolean x, GLboolean y, GLboolean z, GLboolean w) {
			glUniform4i(location, x, y, z, w);
		}

		void ShaderProgram::setUniform(GLint location, const glm::bvec2& v) {
			glUniform2i(location, v.x, v.y);
		}
		
		void ShaderProgram::setUniform(GLint location, const glm::bvec3& v) {
			glUniform3i(location, v.x, v.y, v.z);
		}

		void ShaderProgram::setUniform(GLint location, const glm::bvec4& v) {
			glUniform4i(location, v.x, v.y, v.z, v.w);
		}
			 
		void ShaderProgram::setUniform(GLint location, const glm::mat2& m) {
			glUniformMatrix2fv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::mat3& m) {
			glUniformMatrix3fv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::mat4& m) {
			glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::mat2x3& m) {
			glUniformMatrix2x3fv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::mat2x4& m) {
			glUniformMatrix2x4fv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::mat3x2& m) {
			glUniformMatrix3x2fv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::mat3x4& m) {
			glUniformMatrix3x4fv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::mat4x2& m) {
			glUniformMatrix4x2fv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::mat4x3& m) {
			glUniformMatrix4x3fv(location, 1, GL_FALSE, &m[0][0]);
		}
			 
		void ShaderProgram::setUniform(GLint location, const glm::dmat2& m) {
			glUniformMatrix2dv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::dmat3& m) {
			glUniformMatrix3dv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::dmat4& m) {
			glUniformMatrix4dv(location, 1, GL_FALSE, &m[0][0]);
		}
		
		void ShaderProgram::setUniform(GLint location, const glm::dmat2x3& m) {
			glUniformMatrix2x3dv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::dmat2x4& m) {
			glUniformMatrix2x4dv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::dmat3x2& m) {
			glUniformMatrix3x2dv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::dmat3x4& m) {
			glUniformMatrix3x4dv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::dmat4x2& m) {
			glUniformMatrix4x2dv(location, 1, GL_FALSE, &m[0][0]);
		}

		void ShaderProgram::setUniform(GLint location, const glm::dmat4x3& m) {
			glUniformMatrix4x3dv(location, 1, GL_FALSE, &m[0][0]);
		}

		std::ostream& operator << (std::ostream& os, const ShaderProgram& program) {
			os
				<< "Shader program: "
				<< program.getHandle();

			if (program.isLinked()) {
				program.listAttributes();
				program.listUniforms();
			}

			return os;
		}
	}
}
