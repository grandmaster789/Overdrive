#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include <memory>
#include "../opengl.h"
#include "shader.h"

namespace overdrive {
	namespace render {
		// [NOTE] This uses lazy initialization so that the ShaderProgram can be created on the stack (without an active openGL context)
		//		  The program will be compiled, linked, verified etc upon the first time a shader is attached
		// [NOTE] The amount of uniforms and/or attributes is too low to justify using a mapping structure
		// [TODO] to/from binary formats should be possible

		// http://docs.gl/gl4/glCreateProgram
		class ShaderProgram {
		public:
			ShaderProgram();
			~ShaderProgram();

			ShaderProgram(const ShaderProgram&) = delete;
			ShaderProgram(ShaderProgram&&) = delete;
			ShaderProgram& operator = (const ShaderProgram&) = delete;
			ShaderProgram& operator = (ShaderProgram&&) = delete;

			void attachShader(const std::string& source, eShaderType type);
			void loadShader(const boost::filesystem::path& p, eShaderType type);

			void link();
			bool isLinked() const;
			void validate();

			void bind();	// ~> use
			void unbind();

			void listUniforms();
			void listAttributes();
			
			Shader* getShader(eShaderType type) const;

		private:
			GLuint mHandle;
			std::unique_ptr<Shader> mShaders[6]; // fixed positions for all shader types

			bool mIsLinked;
		};
	}
}