#ifndef OVERDRIVE_RENDER_GLINFO_H
#define OVERDRIVE_RENDER_GLINFO_H

#include <string>
#include "opengl.h"

namespace overdrive {
	namespace render {
		bool checkGL(const char* srcFile, int line); // use preprocessor macros to call this
		void listGLinfo(bool extensionInfo = false);
	}
}

#endif
