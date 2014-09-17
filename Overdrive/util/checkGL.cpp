#include "util/checkGL.h"
#include "core/logger.h"
#include <sstream>
#include "opengl.h"

namespace overdrive {
	namespace util {
		bool checkGL(const char* file, int line) {
			GLenum err = glGetError();
			bool result = true;

			while (err != GL_NO_ERROR) {
				std::ostringstream sstr;
				sstr << "openGL error(" << file << ": " << line << ") ";

				switch (err) {
				case GL_INVALID_ENUM:
					sstr << "Invalid enum";
					break;

				case GL_INVALID_VALUE:
					sstr << "Invalid value";
					break;

				case GL_INVALID_OPERATION:
					sstr << "Invalid operation";
					break;

				case GL_INVALID_FRAMEBUFFER_OPERATION:
					sstr << "Invalid framebuffer operation";
					break;

				case GL_OUT_OF_MEMORY:
					sstr << "Out of memory";
					break;

				default:
					sstr << "Unknown error";
				}

				gLog.error() << sstr.str();

				result = false;
				err = glGetError();
			}

			return result;
		}
	}
}