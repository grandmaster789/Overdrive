#pragma once

#include "shaderprogram.h"

namespace overdrive {
	namespace render {
		template <typename T> 
		void ShaderProgram::setUniform(
			const std::string& name, 
			const T& value
		) {
			GLint location = getUniformLocation(name);
			setUniform(location, value);
		}

		template <typename T> 
		void ShaderProgram::setUniform(
			const std::string& name, 
			const T& x, 
			const T& y
		) {
			GLint location = getUniformLocation(name);
			setUniform(location, x, y);
		}

		template <typename T> 
		void ShaderProgram::setUniform(
			const std::string& name, 
			const T& x, 
			const T& y, 
			const T& z
		) {
			GLint location = getUniformLocation(name);
			setUniform(location, x, y, z);
		}

		template <typename T> 
		void ShaderProgram::setUniform(
			const std::string& name, 
			const T& x, 
			const T& y, 
			const T& z, 
			const T& w
		) {
			GLint location = getUniformLocation(name);
			setUniform(location, x, y, z, w);
		}
	}
}
