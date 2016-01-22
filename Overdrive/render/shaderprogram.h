#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include <boost/container/flat_map.hpp>
#include <ostream>
#include <memory>
#include "../opengl.h"
#include "shader.h"

namespace overdrive {
	namespace render {
		class ShaderAttribute;
		class ShaderUniform;

		// [NOTE] This uses lazy initialization so that the ShaderProgram can be created on the stack (without an active openGL context)
		//		  The program will be compiled, linked, verified etc upon the first time a shader is attached
		// [NOTE] The amount of uniforms and/or attributes is too low to justify using a mapping structure
		// [NOTE] glm also supports a simd optimized vec4 (just the float version), still need to figure out how to properly use it

		// [TODO] the shader program exposes many types of interfaces, perhaps support the remaining types as well?
		// [TODO] to/from binary formats should be possible
		// [TODO] perhaps move the gather active attribute/uniform from this class

		// http://docs.gl/gl4/glCreateProgram
		class ShaderProgram {
		public:
			ShaderProgram();
			~ShaderProgram();

			ShaderProgram(const ShaderProgram&) = delete;
			ShaderProgram(ShaderProgram&&) = delete;
			ShaderProgram& operator = (const ShaderProgram&) = delete;
			ShaderProgram& operator = (ShaderProgram&&) = delete;

			GLuint getHandle() const;

			void attachShader(const std::string& source, eShaderType type);
			void loadShader(const boost::filesystem::path& p, eShaderType type);

			void link();
			bool isLinked() const;
			void validate();

			void bind();	// ~> use
			void unbind();

			void listUniforms() const;
			void listAttributes() const;
			
			Shader* getShader(eShaderType type) const;

			void bindAttributeLocation(GLuint id, const std::string& name);
			void bindFragDataLocation(GLuint id, const std::string& name);

			GLint getUniformLocation(const std::string& name) const;
			const ShaderUniform& getUniformData(const std::string& name) const;
			
			GLint getAttributeLocation(const std::string& name) const;
			const ShaderAttribute& getAttributeData(const std::string& name) const;

			// ------ Set Uniform -----

			template <typename T> void setUniform(const std::string& name, const T& value);
			template <typename T> void setUniform(const std::string& name, const T& x, const T& y);
			template <typename T> void setUniform(const std::string& name, const T& x, const T& y, const T& z);
			template <typename T> void setUniform(const std::string& name, const T& x, const T& y, const T& z, const T& w);

			// float
			void setUniform(GLint location, GLfloat x);
			void setUniform(GLint location, GLfloat x, GLfloat y);
			void setUniform(GLint location, GLfloat x, GLfloat y, GLfloat z);
			void setUniform(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
			void setUniform(GLint location, const glm::vec2& v);
			void setUniform(GLint location, const glm::vec3& v);
			void setUniform(GLint location, const glm::vec4& v);
					
			// double
			void setUniform(GLint location, GLdouble x);
			void setUniform(GLint location, GLdouble x, GLdouble y);
			void setUniform(GLint location, GLdouble x, GLdouble y, GLdouble z);
			void setUniform(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
			void setUniform(GLint location, const glm::dvec2& v);
			void setUniform(GLint location, const glm::dvec3& v);
			void setUniform(GLint location, const glm::dvec4& v);
					
			// int	
			void setUniform(GLint location, GLint x);
			void setUniform(GLint location, GLint x, GLint y);
			void setUniform(GLint location, GLint x, GLint y, GLint z);
			void setUniform(GLint location, GLint x, GLint y, GLint z, GLint w);
			void setUniform(GLint location, const glm::ivec2& v);
			void setUniform(GLint location, const glm::ivec3& v);
			void setUniform(GLint location, const glm::ivec4& v);
							
			// unsigned int	
			void setUniform(GLint location, GLuint x);
			void setUniform(GLint location, GLuint x, GLuint y);
			void setUniform(GLint location, GLuint x, GLuint y, GLuint z);
			void setUniform(GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
			void setUniform(GLint location, const glm::uvec2& v);
			void setUniform(GLint location, const glm::uvec3& v);
			void setUniform(GLint location, const glm::uvec4& v);
							
			// unsigned char
			void setUniform(GLint location, GLboolean x);
			void setUniform(GLint location, GLboolean x, GLboolean y);
			void setUniform(GLint location, GLboolean x, GLboolean y, GLboolean z);
			void setUniform(GLint location, GLboolean x, GLboolean y, GLboolean z, GLboolean w);
			void setUniform(GLint location, const glm::bvec2& v);
			void setUniform(GLint location, const glm::bvec3& v);
			void setUniform(GLint location, const glm::bvec4& v);
							
			// float matrices
			void setUniform(GLint location, const glm::mat2& m);
			void setUniform(GLint location, const glm::mat3& m);
			void setUniform(GLint location, const glm::mat4& m);
			void setUniform(GLint location, const glm::mat2x3& m);
			void setUniform(GLint location, const glm::mat2x4& m);
			void setUniform(GLint location, const glm::mat3x2& m);
			void setUniform(GLint location, const glm::mat3x4& m);
			void setUniform(GLint location, const glm::mat4x2& m);
			void setUniform(GLint location, const glm::mat4x3& m);
							
			// double matrices
			void setUniform(GLint location, const glm::dmat2& m);
			void setUniform(GLint location, const glm::dmat3& m);
			void setUniform(GLint location, const glm::dmat4& m);
			void setUniform(GLint location, const glm::dmat2x3& m);
			void setUniform(GLint location, const glm::dmat2x4& m);
			void setUniform(GLint location, const glm::dmat3x2& m);
			void setUniform(GLint location, const glm::dmat3x4& m);
			void setUniform(GLint location, const glm::dmat4x2& m);
			void setUniform(GLint location, const glm::dmat4x3& m);

			// samplers
			// int_samplers
			// uint_samplers
			// image
			// int_image
			// uint_image
			// atomic counter

		private:
			void gatherUniforms();		// query all active uniforms and store results in mUniforms
			void gatherAttributes();	// query all active attributes and store results in mAttributes

			GLuint mHandle;
			std::unique_ptr<Shader> mShaders[6]; // fixed positions for all shader types
			
			boost::container::flat_map<std::string, ShaderUniform> mUniforms;
			boost::container::flat_map<std::string, ShaderAttribute> mAttributes;

			bool mIsLinked;
		};
		
		std::ostream& operator << (std::ostream& os, const ShaderProgram& program);
	}
}

#include "shaderprogram.inl"