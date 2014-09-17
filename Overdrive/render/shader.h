#ifndef OVERDRIVE_RENDER_SHADER_H
#define OVERDRIVE_RENDER_SHADER_H

#include <string>
#include "opengl.h"

namespace overdrive {
	namespace render {
		enum eShaderType {
			VERTEX,
			FRAGMENT,
			GEOMETRY,
			TESSELATION_CONTROL,
			TESSELATION_EVALUATION,
			COMPUTE
		};

		class Shader {
		public:
			Shader(eShaderType type);
			~Shader();

			bool compile(std::string source);

			GLuint getHandle() const;
			
		private:
			GLuint mShaderHandle;
		};
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::render::eShaderType& type);

#endif
