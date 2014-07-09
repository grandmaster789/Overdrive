#include "render/glinfo.h"
#include "core/logger.h"

namespace overdrive {
	namespace render {
		bool checkGL(const char* srcFile, int line) {
			GLenum err = glGetError();
			bool result = true;

			//consume all error messages in a loop, if any
			while (err != GL_NO_ERROR) {
				switch (err) {
				case GL_INVALID_ENUM:
					gLog.error() << "(GL) Invalid enum: " << srcFile << ", line " << line;
					break;

				case GL_INVALID_VALUE:
					gLog.error() << "(GL) Invalid value: " << srcFile << ", line " << line;
					break;

				case GL_INVALID_OPERATION:
					gLog.error() << "(GL) Invalid operation: " << srcFile << ", line " << line;
					break;

				case GL_INVALID_FRAMEBUFFER_OPERATION:
					gLog.error() << "(GL) Invalid framebuffer operation: " << srcFile << ", line " << line;
					break;

				case GL_OUT_OF_MEMORY:
					gLog.error() << "(GL) Out of memory: " << srcFile << ", line " << line;
					break;

				default:
					gLog.error() << "(GL) Unknown error: " << srcFile << ", line " << line;
				}
				
				result = false;
				err = glGetError();
			}

			return result;
		}

		void listGLinfo(bool extensionInfo) {
			auto vendor = glGetString(GL_VENDOR);
			auto renderer = glGetString(GL_RENDERER);
			auto version = glGetString(GL_VERSION);
			auto shaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

			GLint major;
			GLint minor;
			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);

			gLog << "openGL " << version << " " << major << "." << minor;
			gLog << "GLSL v" << shaderVersion;
			gLog << "openGL Vendor: " << vendor;
			gLog << "openGL Renderer: " << renderer;

			GLint numExtensions;
			glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

			gLog << numExtensions << " extensions available";

			if (extensionInfo) {
				for (GLint i = 0; i < numExtensions; ++i)
					gLog << glGetStringi(GL_EXTENSIONS, i);
			}
		}
	}
}