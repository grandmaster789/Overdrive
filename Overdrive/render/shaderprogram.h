#ifndef OVERDRIVE_RENDER_SHADERPROGRAM_H
#define OVERDRIVE_RENDER_SHADERPROGRAM_H

#include "opengl.h"
#include "util/unique_resource.h"

#include <array>
#include <string>
#include <unordered_map>

namespace overdrive {
	namespace render {
		enum eShader {
			VERTEX = 0,
			FRAGMENT,
			GEOMETRY,
			TESSELATION_CONTROL,
			TESSELATION_EVALUATION
		};

		class ShaderProgram {
		public:
			ShaderProgram();
			~ShaderProgram(); 

			// no copies
			ShaderProgram(const ShaderProgram&) = delete;
			ShaderProgram& operator = (const ShaderProgram&) = delete;

			ShaderProgram(ShaderProgram&& sp);
			ShaderProgram& operator = (ShaderProgram&& sp);

			bool compileFile(const std::string& filename, eShader type);
			bool compileString(const std::string& source, eShader type);
			bool link();
			bool validate() const;
			
			void use() const;
			GLuint getHandle() const;

			bool isLinked() const;

			void set(const std::string& name, float x, float y, float z) const;
			void set(const std::string& name, const glm::vec2& v) const;
			void set(const std::string& name, const glm::vec3& v) const;
			void set(const std::string& name, const glm::vec4& v) const;
			void set(const std::string& name, const glm::mat3& m) const;
			void set(const std::string& name, const glm::mat4& m) const;
			void set(const std::string& name, float f) const;
			void set(const std::string& name, int i) const;
			void set(const std::string& name, bool b) const;

			void listActiveUniforms() const;
			void listActiveAttributes() const;

		private:
			bool exists(const std::string& filename) const;

			void gatherActiveAttributes();
			void gatherActiveUniforms();

			inline GLint getUniformLocation(const std::string& name) const;
			inline GLint getAttributeLocation(const std::string& name) const;
						
			bool mIsLinked;
			
			GLuint mProgramHandle;
			std::array<GLuint, 5> mShaderHandles;

			std::unordered_map<std::string, GLint> mUniformLocations;
			std::unordered_map<std::string, GLint> mAttributeLocations;
		};

		GLint ShaderProgram::getUniformLocation(const std::string& name) const {
			//GLint result = glGetUniformLocation(mProgramHandle, name.c_str());

			auto it = mUniformLocations.find(name);
			
			if (it == mUniformLocations.end())
				throw std::runtime_error(std::string("Uniform not found: ") + name);

			return it->second;
		}

		GLint ShaderProgram::getAttributeLocation(const std::string& name) const {
			auto it = mAttributeLocations.find(name);

			if (it == mAttributeLocations.end())
				throw std::runtime_error(std::string("Attribute not found: ") + name);

			return it->second;
		}
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::render::eShader& shaderType);

#endif
