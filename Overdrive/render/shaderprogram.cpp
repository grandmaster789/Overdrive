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

		// http://docs.gl/gl4/glGetActiveUniform
		void ShaderProgram::listUniforms() const {
			GLint numUniforms = 0;
			GLint maxUniformNameLength = 0;
			GLint location = 0;
			GLenum type;
			GLint size = 0;
			GLint bytesWritten = 0;

			glGetProgramiv(mHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
			glGetProgramiv(mHandle, GL_ACTIVE_UNIFORMS, &numUniforms);

			std::unique_ptr<GLchar[]> uniformName(new GLchar[maxUniformNameLength]);

			gLogDebug << " ---- Active Uniforms ---- ";

			for (GLint i = 0; i < numUniforms; ++i) {
				glGetActiveUniform(
					mHandle,
					i,
					maxUniformNameLength,
					&bytesWritten,
					&size,
					&type,
					uniformName.get()
				);

				location = glGetUniformLocation(mHandle, uniformName.get());

				gLog << i <<
					":\t" << uniformName.get() <<
					" (" << toUniformType(type) << ", " << size << ") " <<
					"=> " << location;
			}

			gLogDebug << " ------------------------- ";
		}

		// http://docs.gl/gl4/glGetActiveAttrib
		void ShaderProgram::listAttributes() const {
			GLint numAttributes = 0;
			GLint maxAttributeNameLength = 0;
			GLint location = 0;
			GLenum type;
			GLint size = 0;
			GLint bytesWritten = 0;

			glGetProgramiv(mHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);
			glGetProgramiv(mHandle, GL_ACTIVE_ATTRIBUTES, &numAttributes);

			std::unique_ptr<GLchar[]> attributeName(new GLchar[maxAttributeNameLength]);

			gLogDebug << " ---- Active Attributes ---- ";

			for (GLint i = 0; i < numAttributes; ++i) {
				glGetActiveAttrib(
					mHandle,
					i,
					maxAttributeNameLength,
					&bytesWritten,
					&size,
					&type,
					attributeName.get()
				);

				location = glGetAttribLocation(mHandle, attributeName.get());

				gLog << i <<
					":\t" << attributeName.get() <<
					" (" << toAttributeType(type) << ", " << size << ") " <<
					"=> " << location;
			}

			gLogDebug << " ------------------------- ";
		}

		Shader* ShaderProgram::getShader(eShaderType type) const {
			return mShaders[getShaderIndex(type)].get();
		}

		eUniformType toUniformType(GLenum value) {
			switch (value) {
			case GL_FLOAT: return eUniformType::FLOAT;
			case GL_FLOAT_VEC2: return eUniformType::FLOAT_VEC2;
			case GL_FLOAT_VEC3: return eUniformType::FLOAT_VEC3;
			case GL_FLOAT_VEC4: return eUniformType::FLOAT_VEC4;

			case GL_DOUBLE: return eUniformType::DOUBLE;
			case GL_DOUBLE_VEC2: return eUniformType::DOUBLE_VEC2;
			case GL_DOUBLE_VEC3: return eUniformType::DOUBLE_VEC3;
			case GL_DOUBLE_VEC4: return eUniformType::DOUBLE_VEC4;
			
			case GL_INT: return eUniformType::INT;
			case GL_INT_VEC2: return eUniformType::INT_VEC2;
			case GL_INT_VEC3: return eUniformType::INT_VEC3;
			case GL_INT_VEC4: return eUniformType::INT_VEC4;

			case GL_UNSIGNED_INT: return eUniformType::UNSIGNED_INT;
			case GL_UNSIGNED_INT_VEC2: return eUniformType::UNSIGNED_INT_VEC2;
			case GL_UNSIGNED_INT_VEC3: return eUniformType::UNSIGNED_INT_VEC3;
			case GL_UNSIGNED_INT_VEC4: return eUniformType::UNSIGNED_INT_VEC4;
			
			case GL_BOOL: return eUniformType::BOOL;
			case GL_BOOL_VEC2: return eUniformType::BOOL_VEC2;
			case GL_BOOL_VEC3: return eUniformType::BOOL_VEC3;
			case GL_BOOL_VEC4: return eUniformType::BOOL_VEC4;
			
			case GL_FLOAT_MAT2: return eUniformType::FLOAT_MAT2;
			case GL_FLOAT_MAT3: return eUniformType::FLOAT_MAT3;
			case GL_FLOAT_MAT4: return eUniformType::FLOAT_MAT4;
			case GL_FLOAT_MAT2x3: return eUniformType::FLOAT_MAT2x3;
			case GL_FLOAT_MAT2x4: return eUniformType::FLOAT_MAT2x4;
			case GL_FLOAT_MAT3x2: return eUniformType::FLOAT_MAT3x2;
			case GL_FLOAT_MAT3x4: return eUniformType::FLOAT_MAT3x4;
			case GL_FLOAT_MAT4x2: return eUniformType::FLOAT_MAT4x2;
			case GL_FLOAT_MAT4x3: return eUniformType::FLOAT_MAT4x3;

			case GL_DOUBLE_MAT2: return eUniformType::DOUBLE_MAT2;
			case GL_DOUBLE_MAT3: return eUniformType::DOUBLE_MAT3;
			case GL_DOUBLE_MAT4: return eUniformType::DOUBLE_MAT4;
			case GL_DOUBLE_MAT2x3: return eUniformType::DOUBLE_MAT2x3;
			case GL_DOUBLE_MAT2x4: return eUniformType::DOUBLE_MAT2x4;
			case GL_DOUBLE_MAT3x2: return eUniformType::DOUBLE_MAT3x2;
			case GL_DOUBLE_MAT3x4: return eUniformType::DOUBLE_MAT3x4;
			case GL_DOUBLE_MAT4x2: return eUniformType::DOUBLE_MAT4x2;
			case GL_DOUBLE_MAT4x3: return eUniformType::DOUBLE_MAT4x3;

			case GL_SAMPLER_1D: return eUniformType::SAMPLER_1D;
			case GL_SAMPLER_2D: return eUniformType::SAMPLER_2D;
			case GL_SAMPLER_3D: return eUniformType::SAMPLER_3D;
			case GL_SAMPLER_CUBE: return eUniformType::SAMPLER_CUBE;
			case GL_SAMPLER_1D_SHADOW: return eUniformType::SAMPLER_1D_SHADOW;
			case GL_SAMPLER_2D_SHADOW: return eUniformType::SAMPLER_2D_SHADOW;
			case GL_SAMPLER_1D_ARRAY: return eUniformType::SAMPLER_1D_ARRAY;
			case GL_SAMPLER_2D_ARRAY: return eUniformType::SAMPLER_2D_ARRAY;
			case GL_SAMPLER_1D_ARRAY_SHADOW: return eUniformType::SAMPLER_1D_ARRAY_SHADOW;
			case GL_SAMPLER_2D_ARRAY_SHADOW: return eUniformType::SAMPLER_2D_ARRAY_SHADOW;
			case GL_SAMPLER_2D_MULTISAMPLE: return eUniformType::SAMPLER_2D_MULTISAMPLE;
			case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return eUniformType::SAMPLER_2D_MULTISAMPLE_ARRAY;
			case GL_SAMPLER_CUBE_SHADOW: return eUniformType::SAMPLER_CUBE_SHADOW;
			case GL_SAMPLER_BUFFER: return eUniformType::SAMPLER_BUFFER;
			case GL_SAMPLER_2D_RECT: return eUniformType::SAMPLER_2D_RECT;
			case GL_SAMPLER_2D_RECT_SHADOW: return eUniformType::SAMPLER_2D_RECT_SHADOW;
			
			case GL_INT_SAMPLER_1D: return eUniformType::INT_SAMPLER_1D;
			case GL_INT_SAMPLER_2D: return eUniformType::INT_SAMPLER_2D;
			case GL_INT_SAMPLER_3D: return eUniformType::INT_SAMPLER_3D;
			case GL_INT_SAMPLER_CUBE: return eUniformType::INT_SAMPLER_CUBE;
			case GL_INT_SAMPLER_1D_ARRAY: return eUniformType::INT_SAMPLER_1D_ARRAY;
			case GL_INT_SAMPLER_2D_ARRAY: return eUniformType::INT_SAMPLER_2D_ARRAY;
			case GL_INT_SAMPLER_2D_MULTISAMPLE: return eUniformType::INT_SAMPLER_2D_MULTISAMPLE;
			case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return eUniformType::INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
			case GL_INT_SAMPLER_BUFFER: return eUniformType::INT_SAMPLER_BUFFER;
			case GL_INT_SAMPLER_2D_RECT: return eUniformType::INT_SAMPLER_2D_RECT;

			case GL_UNSIGNED_INT_SAMPLER_1D: return eUniformType::UNSIGNED_INT_SAMPLER_1D;
			case GL_UNSIGNED_INT_SAMPLER_2D: return eUniformType::UNSIGNED_INT_SAMPLER_2D;
			case GL_UNSIGNED_INT_SAMPLER_3D: return eUniformType::UNSIGNED_INT_SAMPLER_3D;
			case GL_UNSIGNED_INT_SAMPLER_CUBE: return eUniformType::UNSIGNED_INT_SAMPLER_CUBE;
			case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: return eUniformType::UNSIGNED_INT_SAMPLER_1D_ARRAY;
			case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return eUniformType::UNSIGNED_INT_SAMPLER_2D_ARRAY;
			case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return eUniformType::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE;
			case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return eUniformType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
			case GL_UNSIGNED_INT_SAMPLER_BUFFER: return eUniformType::UNSIGNED_INT_SAMPLER_BUFFER;
			case GL_UNSIGNED_INT_SAMPLER_2D_RECT: return eUniformType::UNSIGNED_INT_SAMPLER_2D_RECT;

			case GL_IMAGE_1D: return eUniformType::IMAGE_1D;
			case GL_IMAGE_2D: return eUniformType::IMAGE_2D;
			case GL_IMAGE_3D: return eUniformType::IMAGE_3D;
			case GL_IMAGE_2D_RECT: return eUniformType::IMAGE_2D_RECT;
			case GL_IMAGE_CUBE: return eUniformType::IMAGE_CUBE;
			case GL_IMAGE_BUFFER: return eUniformType::IMAGE_BUFFER;
			case GL_IMAGE_1D_ARRAY: return eUniformType::IMAGE_1D_ARRAY;
			case GL_IMAGE_2D_ARRAY: return eUniformType::IMAGE_2D_ARRAY;
			case GL_IMAGE_2D_MULTISAMPLE: return eUniformType::IMAGE_2D_MULTISAMPLE;
			case GL_IMAGE_2D_MULTISAMPLE_ARRAY: return eUniformType::IMAGE_2D_MULTISAMPLE_ARRAY;

			case GL_INT_IMAGE_1D: return eUniformType::INT_IMAGE_1D;
			case GL_INT_IMAGE_2D: return eUniformType::INT_IMAGE_2D;
			case GL_INT_IMAGE_3D: return eUniformType::INT_IMAGE_3D;
			case GL_INT_IMAGE_2D_RECT: return eUniformType::INT_IMAGE_2D_RECT;
			case GL_INT_IMAGE_CUBE: return eUniformType::INT_IMAGE_CUBE;
			case GL_INT_IMAGE_BUFFER: return eUniformType::INT_IMAGE_BUFFER;
			case GL_INT_IMAGE_1D_ARRAY: return eUniformType::INT_IMAGE_1D_ARRAY;
			case GL_INT_IMAGE_2D_ARRAY: return eUniformType::INT_IMAGE_2D_ARRAY;
			case GL_INT_IMAGE_2D_MULTISAMPLE: return eUniformType::INT_IMAGE_2D_MULTISAMPLE;
			case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return eUniformType::INT_IMAGE_2D_MULTISAMPLE_ARRAY;

			case GL_UNSIGNED_INT_IMAGE_1D: return eUniformType::UNSIGNED_INT_IMAGE_1D;
			case GL_UNSIGNED_INT_IMAGE_2D: return eUniformType::UNSIGNED_INT_IMAGE_2D;
			case GL_UNSIGNED_INT_IMAGE_3D: return eUniformType::UNSIGNED_INT_IMAGE_3D;
			case GL_UNSIGNED_INT_IMAGE_2D_RECT: return eUniformType::UNSIGNED_INT_IMAGE_2D_RECT;
			case GL_UNSIGNED_INT_IMAGE_CUBE: return eUniformType::UNSIGNED_INT_IMAGE_CUBE;
			case GL_UNSIGNED_INT_IMAGE_BUFFER: return eUniformType::UNSIGNED_INT_IMAGE_BUFFER;
			case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: return eUniformType::UNSIGNED_INT_IMAGE_1D_ARRAY;
			case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: return eUniformType::UNSIGNED_INT_IMAGE_2D_ARRAY;
			case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: return eUniformType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE;
			case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return eUniformType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY;

			case GL_UNSIGNED_INT_ATOMIC_COUNTER: return eUniformType::UNSIGNED_INT_ATOMIC_COUNTER;

			default:
				throw ShaderException("Unsupported uniform type");
			}
		}

		eAttributeType toAttributeType(GLenum value) {
			switch (value) {
			case GL_FLOAT: return eAttributeType::FLOAT;
			case GL_FLOAT_VEC2: return eAttributeType::FLOAT_VEC2;
			case GL_FLOAT_VEC3: return eAttributeType::FLOAT_VEC3;
			case GL_FLOAT_VEC4: return eAttributeType::FLOAT_VEC4;

			case GL_FLOAT_MAT2: return eAttributeType::FLOAT_MAT2;
			case GL_FLOAT_MAT3: return eAttributeType::FLOAT_MAT3;
			case GL_FLOAT_MAT4: return eAttributeType::FLOAT_MAT4;
			case GL_FLOAT_MAT2x3: return eAttributeType::FLOAT_MAT2x3;
			case GL_FLOAT_MAT2x4: return eAttributeType::FLOAT_MAT2x4;
			case GL_FLOAT_MAT3x2: return eAttributeType::FLOAT_MAT3x2;
			case GL_FLOAT_MAT3x4: return eAttributeType::FLOAT_MAT3x4;
			case GL_FLOAT_MAT4x2: return eAttributeType::FLOAT_MAT4x2;
			case GL_FLOAT_MAT4x3: return eAttributeType::FLOAT_MAT4x3;
			
			case GL_INT: return eAttributeType::INT;
			case GL_INT_VEC2: return eAttributeType::INT_VEC2;
			case GL_INT_VEC3: return eAttributeType::INT_VEC3;
			case GL_INT_VEC4: return eAttributeType::INT_VEC4;
			
			case GL_UNSIGNED_INT: return eAttributeType::UNSIGNED_INT;
			case GL_UNSIGNED_INT_VEC2: return eAttributeType::UNSIGNED_INT_VEC2;
			case GL_UNSIGNED_INT_VEC3: return eAttributeType::UNSIGNED_INT_VEC3;
			case GL_UNSIGNED_INT_VEC4: return eAttributeType::UNSIGNED_INT_VEC4;
			
			case GL_DOUBLE: return eAttributeType::DOUBLE;
			case GL_DOUBLE_VEC2: return eAttributeType::DOUBLE_VEC2;
			case GL_DOUBLE_VEC3: return eAttributeType::DOUBLE_VEC3;
			case GL_DOUBLE_VEC4: return eAttributeType::DOUBLE_VEC4;
			
			case GL_DOUBLE_MAT2: return eAttributeType::DOUBLE_MAT2;
			case GL_DOUBLE_MAT3: return eAttributeType::DOUBLE_MAT3;
			case GL_DOUBLE_MAT4: return eAttributeType::DOUBLE_MAT4;
			case GL_DOUBLE_MAT2x3: return eAttributeType::DOUBLE_MAT2x3;
			case GL_DOUBLE_MAT2x4: return eAttributeType::DOUBLE_MAT2x4;
			case GL_DOUBLE_MAT3x2: return eAttributeType::DOUBLE_MAT3x2;
			case GL_DOUBLE_MAT3x4: return eAttributeType::DOUBLE_MAT3x4;
			case GL_DOUBLE_MAT4x2: return eAttributeType::DOUBLE_MAT4x2;
			case GL_DOUBLE_MAT4x3: return eAttributeType::DOUBLE_MAT4x3;

			default:
				throw ShaderException("Unsupported attribute type");
			}
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

		std::ostream& operator << (std::ostream& os, const eUniformType& type) {
			switch (type) {
			case eUniformType::FLOAT: os << "float"; break;
				case eUniformType::FLOAT_VEC2: os << "vec2"; break;
				case eUniformType::FLOAT_VEC3: os << "vec3"; break;
				case eUniformType::FLOAT_VEC4: os << "vec4"; break;

				case eUniformType::DOUBLE: os << "double"; break;
				case eUniformType::DOUBLE_VEC2: os << "dvec2"; break;
				case eUniformType::DOUBLE_VEC3: os << "dvec3"; break;
				case eUniformType::DOUBLE_VEC4: os << "dvec4"; break;
				
				case eUniformType::INT: os << "int"; break;
				case eUniformType::INT_VEC2: os << "ivec2"; break;
				case eUniformType::INT_VEC3: os << "ivec3"; break;
				case eUniformType::INT_VEC4: os << "ivec4"; break;
				
				case eUniformType::UNSIGNED_INT: os << "unsigned int"; break;
				case eUniformType::UNSIGNED_INT_VEC2: os << "uvec2"; break;
				case eUniformType::UNSIGNED_INT_VEC3: os << "uvec3"; break;
				case eUniformType::UNSIGNED_INT_VEC4: os << "uvec4"; break;
				
				case eUniformType::BOOL: os << "bool"; break;
				case eUniformType::BOOL_VEC2: os << "bvec2"; break;
				case eUniformType::BOOL_VEC3: os << "bvec3"; break;
				case eUniformType::BOOL_VEC4: os << "bvec4"; break;
				
				case eUniformType::FLOAT_MAT2: os << "mat2"; break;
				case eUniformType::FLOAT_MAT3: os << "mat3"; break;
				case eUniformType::FLOAT_MAT4: os << "mat4"; break;
				case eUniformType::FLOAT_MAT2x3: os << "mat2x3"; break;
				case eUniformType::FLOAT_MAT2x4: os << "mat2x4"; break;
				case eUniformType::FLOAT_MAT3x2: os << "mat3x2"; break;
				case eUniformType::FLOAT_MAT3x4: os << "mat3x4"; break;
				case eUniformType::FLOAT_MAT4x2: os << "mat4x2"; break;
				case eUniformType::FLOAT_MAT4x3: os << "mat4x3"; break;
				
				case eUniformType::DOUBLE_MAT2: os << "dmat2"; break;
				case eUniformType::DOUBLE_MAT3: os << "dmat3"; break;
				case eUniformType::DOUBLE_MAT4: os << "dmat4"; break;
				case eUniformType::DOUBLE_MAT2x3: os << "dmat2x3"; break;
				case eUniformType::DOUBLE_MAT2x4: os << "dmat2x4"; break;
				case eUniformType::DOUBLE_MAT3x2: os << "dmat3x2"; break;
				case eUniformType::DOUBLE_MAT3x4: os << "dmat3x4"; break;
				case eUniformType::DOUBLE_MAT4x2: os << "dmat4x2"; break;
				case eUniformType::DOUBLE_MAT4x3: os << "dmat4x3"; break;
				
				case eUniformType::SAMPLER_1D: os << "sampler1D"; break;
				case eUniformType::SAMPLER_2D: os << "sampler2D"; break;
				case eUniformType::SAMPLER_3D: os << "sampler3D"; break;
				case eUniformType::SAMPLER_CUBE: os << "samplerCube"; break;
				case eUniformType::SAMPLER_1D_SHADOW: os << "sampler1DShadow"; break;
				case eUniformType::SAMPLER_2D_SHADOW: os << "sampler2DShadow"; break;
				case eUniformType::SAMPLER_1D_ARRAY: os << "sampler1DArray"; break;
				case eUniformType::SAMPLER_2D_ARRAY: os << "sampler2DArray"; break;
				case eUniformType::SAMPLER_1D_ARRAY_SHADOW: os << "sampler1DArrayShadow"; break;
				case eUniformType::SAMPLER_2D_ARRAY_SHADOW: os << "sampler2DArrayShadow"; break;
				case eUniformType::SAMPLER_2D_MULTISAMPLE: os << "sampler2DMS"; break;
				case eUniformType::SAMPLER_2D_MULTISAMPLE_ARRAY: os << "sampler2DMSArray"; break;
				case eUniformType::SAMPLER_CUBE_SHADOW: os << "samplerCubeShadow"; break;
				case eUniformType::SAMPLER_BUFFER: os << "samplerBuffer"; break;
				case eUniformType::SAMPLER_2D_RECT: os << "sampler2DRect"; break;
				case eUniformType::SAMPLER_2D_RECT_SHADOW: os << "sampler2DRectShadow"; break;
				
				case eUniformType::INT_SAMPLER_1D: os << "isampler1D"; break;
				case eUniformType::INT_SAMPLER_2D: os << "isampler2D"; break;
				case eUniformType::INT_SAMPLER_3D: os << "isampler3D"; break;
				case eUniformType::INT_SAMPLER_CUBE: os << "isamplerCube"; break;
				case eUniformType::INT_SAMPLER_1D_ARRAY: os << "isampler1DArray"; break;
				case eUniformType::INT_SAMPLER_2D_ARRAY: os << "isampler2DArray"; break;
				case eUniformType::INT_SAMPLER_2D_MULTISAMPLE: os << "isampler2DMS"; break;
				case eUniformType::INT_SAMPLER_2D_MULTISAMPLE_ARRAY: os << "isampler2DMSArray"; break;
				case eUniformType::INT_SAMPLER_BUFFER: os << "isamplerBuffer"; break;
				case eUniformType::INT_SAMPLER_2D_RECT: os << "isampler2DRect"; break;
				
				case eUniformType::UNSIGNED_INT_SAMPLER_1D: os << "usampler1D"; break;
				case eUniformType::UNSIGNED_INT_SAMPLER_2D: os << "usampler2D"; break;
				case eUniformType::UNSIGNED_INT_SAMPLER_3D: os << "usampler3D"; break;
				case eUniformType::UNSIGNED_INT_SAMPLER_CUBE: os << "usamplerCube"; break;
				case eUniformType::UNSIGNED_INT_SAMPLER_1D_ARRAY: os << "usampler1DArray"; break; // [NOTE] weirdly this is listed as usampler2Darray both on docs.gl and opengl.org
				case eUniformType::UNSIGNED_INT_SAMPLER_2D_ARRAY: os << "usampler2DArray"; break;
				case eUniformType::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: os << "usampler2DMS"; break;
				case eUniformType::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: os << "usampler2DMSArray"; break;
				case eUniformType::UNSIGNED_INT_SAMPLER_BUFFER: os << "usamplerBuffer"; break;
				case eUniformType::UNSIGNED_INT_SAMPLER_2D_RECT: os << "usampler2DRect"; break;
				
				case eUniformType::IMAGE_1D: os << "image1D"; break;
				case eUniformType::IMAGE_2D: os << "image2D"; break;
				case eUniformType::IMAGE_3D: os << "image3D"; break;
				case eUniformType::IMAGE_2D_RECT: os << "image2DRect"; break;
				case eUniformType::IMAGE_CUBE: os << "imageCube"; break;
				case eUniformType::IMAGE_BUFFER: os << "imageBuffer"; break;
				case eUniformType::IMAGE_1D_ARRAY: os << "image1DArray"; break;
				case eUniformType::IMAGE_2D_ARRAY: os << "image2DArray"; break;
				case eUniformType::IMAGE_2D_MULTISAMPLE: os << "image2DMS"; break;
				case eUniformType::IMAGE_2D_MULTISAMPLE_ARRAY: os << "image2DMSArray"; break;
				
				case eUniformType::INT_IMAGE_1D: os << "iimage1D"; break;
				case eUniformType::INT_IMAGE_2D: os << "iimage2D"; break;
				case eUniformType::INT_IMAGE_3D: os << "iimage3D"; break;
				case eUniformType::INT_IMAGE_2D_RECT: os << "iimage2DRect"; break;
				case eUniformType::INT_IMAGE_CUBE: os << "iimageCube"; break;
				case eUniformType::INT_IMAGE_BUFFER: os << "iimageBuffer"; break;
				case eUniformType::INT_IMAGE_1D_ARRAY: os << "iimage1DArray"; break;
				case eUniformType::INT_IMAGE_2D_ARRAY: os << "iimage2DArray"; break;
				case eUniformType::INT_IMAGE_2D_MULTISAMPLE: os << "iimage2DMS"; break;
				case eUniformType::INT_IMAGE_2D_MULTISAMPLE_ARRAY: os << "iimage2DMSArray"; break;
				
				case eUniformType::UNSIGNED_INT_IMAGE_1D: os << "uimage1D"; break;
				case eUniformType::UNSIGNED_INT_IMAGE_2D: os << "uimage2D"; break;
				case eUniformType::UNSIGNED_INT_IMAGE_3D: os << "uimage3D"; break;
				case eUniformType::UNSIGNED_INT_IMAGE_2D_RECT: os << "uimage2DRect"; break;
				case eUniformType::UNSIGNED_INT_IMAGE_CUBE: os << "uimageCube"; break;
				case eUniformType::UNSIGNED_INT_IMAGE_BUFFER: os << "uimageBuffer"; break;
				case eUniformType::UNSIGNED_INT_IMAGE_1D_ARRAY: os << "uimage1DArray"; break;
				case eUniformType::UNSIGNED_INT_IMAGE_2D_ARRAY: os << "uimage2DArray"; break;
				case eUniformType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: os << "uimage2DMS"; break;
				case eUniformType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: os << "uimage2DMSArray"; break;
				
				case eUniformType::UNSIGNED_INT_ATOMIC_COUNTER: os << "atomic_uint"; break;

				default:
					os << "Unknown uniform type: " << static_cast<std::underlying_type<eUniformType>::type>(type);
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eAttributeType& type) {

			switch (type) {
			case eAttributeType::FLOAT: os << "FLOAT"; break;
			case eAttributeType::FLOAT_VEC2: os << "FLOAT_VEC2"; break;
			case eAttributeType::FLOAT_VEC3: os << "FLOAT_VEC3"; break;
			case eAttributeType::FLOAT_VEC4: os << "FLOAT_VEC4"; break;
			
			case eAttributeType::FLOAT_MAT2: os << "FLOAT_MAT2"; break;
			case eAttributeType::FLOAT_MAT3: os << "FLOAT_MAT3"; break;
			case eAttributeType::FLOAT_MAT4: os << "FLOAT_MAT4"; break;
			case eAttributeType::FLOAT_MAT2x3: os << "FLOAT_MAT2x3"; break;
			case eAttributeType::FLOAT_MAT2x4: os << "FLOAT_MAT2x4"; break;
			case eAttributeType::FLOAT_MAT3x2: os << "FLOAT_MAT3x2"; break;
			case eAttributeType::FLOAT_MAT3x4: os << "FLOAT_MAT3x4"; break;
			case eAttributeType::FLOAT_MAT4x2: os << "FLOAT_MAT4x2"; break;
			case eAttributeType::FLOAT_MAT4x3: os << "FLOAT_MAT4x3"; break;
			
			case eAttributeType::INT: os << "INT"; break;
			case eAttributeType::INT_VEC2: os << "INT_VEC2"; break;
			case eAttributeType::INT_VEC3: os << "INT_VEC3"; break;
			case eAttributeType::INT_VEC4: os << "INT_VEC4"; break;
			
			case eAttributeType::UNSIGNED_INT: os << "UNSIGNED_INT"; break;
			case eAttributeType::UNSIGNED_INT_VEC2: os << "UNSIGNED_INT_VEC2"; break;
			case eAttributeType::UNSIGNED_INT_VEC3: os << "UNSIGNED_INT_VEC3"; break;
			case eAttributeType::UNSIGNED_INT_VEC4: os << "UNSIGNED_INT_VEC4"; break;
			
			case eAttributeType::DOUBLE: os << "DOUBLE"; break;
			case eAttributeType::DOUBLE_VEC2: os << "DOUBLE_VEC2"; break;
			case eAttributeType::DOUBLE_VEC3: os << "DOUBLE_VEC3"; break;
			case eAttributeType::DOUBLE_VEC4: os << "DOUBLE_VEC4"; break;
			
			case eAttributeType::DOUBLE_MAT2: os << "DOUBLE_MAT2"; break;
			case eAttributeType::DOUBLE_MAT3: os << "DOUBLE_MAT3"; break;
			case eAttributeType::DOUBLE_MAT4: os << "DOUBLE_MAT4"; break;
			case eAttributeType::DOUBLE_MAT2x3: os << "DOUBLE_MAT2x3"; break;
			case eAttributeType::DOUBLE_MAT2x4: os << "DOUBLE_MAT2x4"; break;
			case eAttributeType::DOUBLE_MAT3x2: os << "DOUBLE_MAT3x2"; break;
			case eAttributeType::DOUBLE_MAT3x4: os << "DOUBLE_MAT3x4"; break;
			case eAttributeType::DOUBLE_MAT4x2: os << "DOUBLE_MAT4x2"; break;
			case eAttributeType::DOUBLE_MAT4x3: os << "DOUBLE_MAT4x3"; break;

			default:
				os << "Unknown attribute type: " << static_cast<std::underlying_type<eAttributeType>::type>(type);
			}

			return os;
		}
	}
}
