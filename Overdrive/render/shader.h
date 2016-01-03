#pragma once

#include <stdexcept>
#include <string>
#include "../opengl.h"

namespace overdrive {
	namespace render {
		struct ShaderException: std::runtime_error {
			ShaderException(const std::string& description);
		};

		enum class eShaderType: GLenum {
			VERTEX				= GL_VERTEX_SHADER,
			FRAGMENT			= GL_FRAGMENT_SHADER,
			GEOMETRY			= GL_GEOMETRY_SHADER,
			TESSELATION_CONTROL = GL_TESS_CONTROL_SHADER,
			TESSELATION_EVAL	= GL_TESS_EVALUATION_SHADER,
			COMPUTE				= GL_COMPUTE_SHADER
		};

		class Shader {
		public:
			Shader(eShaderType type);
			~Shader();

			Shader(const Shader&) = delete;
			Shader(Shader&& s);
			Shader& operator = (const Shader&) = delete;
			Shader& operator = (Shader&& s);

			GLuint getHandle() const;

			void compile(const std::string& source);

			std::string getSource() const;

		private:
			eShaderType mType;
			GLuint mHandle;
		};

		std::ostream& operator << (std::ostream& os, const eShaderType& type);
	}
}

