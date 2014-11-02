#ifndef OVERDRIVE_RENDER_SHADERPROGRAM_H
#define OVERDRIVE_RENDER_SHADERPROGRAM_H

#include "opengl.h"
#include "render/shader.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <boost/noncopyable.hpp>

namespace overdrive {
	namespace render {
		class ShaderProgram:
			boost::noncopyable
		{
		public:
			ShaderProgram();
			~ShaderProgram();

			bool addShaderFile(std::string filename, eShaderType type);
			bool addShaderSource(std::string source, eShaderType type);

			bool link();
			bool validate();
			
			void use() const;
			bool isLinked() const;

			void printUniforms(); // prints uniform names and locations to log
			void printAttributes(); // prints attribute names and locations to log

			// set uniform variable to value
			void set(const std::string& name, float x, float y, float z) const;
			void set(const std::string& name, const glm::vec4& v) const;
			void set(const std::string& name, const glm::vec3& v) const;
			void set(const std::string& name, const glm::mat3& m) const;
			void set(const std::string& name, const glm::mat4& m) const;
			void set(const std::string& name, float f) const;
			void set(const std::string& name, int i) const;
			void set(const std::string& name, bool b) const;
			void set(const std::string& name, const glm::vec2& v) const;

		private:
			void gatherUniforms();
			void gatherAttributes();

			inline GLint getUniformLocation(std::string name) const;
			inline GLint getAttributeLocation(std::string name) const;

			GLuint mProgramHandle;
			std::unique_ptr<Shader> mProgramShaders[6]; // fixed positions for all shader types
			
			std::unordered_map<std::string, GLint> mUniformLocations;
			std::unordered_map<std::string, GLint> mAttributeLocations;

			bool mIsLinked;
		};

		GLint ShaderProgram::getUniformLocation(std::string name) const {
			auto it = mUniformLocations.find(std::move(name));
			assert(it != mUniformLocations.end());
			return it->second;
		}

		GLint ShaderProgram::getAttributeLocation(std::string name) const {
			auto it = mAttributeLocations.find(std::move(name));
			assert(it != mAttributeLocations.end());
			return it->second;
		}
	}
}

#endif
