#ifndef OVERDRIVE_UTIL_GLINFO_H
#define OVERDRIVE_UTIL_GLINFO_H

#include <string>
#include "opengl.h"

namespace overdrive {
	namespace util {
		bool checkGL(const char* srcFile, int line); // use preprocessor macros to call this
		void listGLinfo(bool extensionInfo = false);
	}
}

#endif
