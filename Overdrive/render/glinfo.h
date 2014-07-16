#ifndef OVERDRIVE_RENDER_GLINFO_H
#define OVERDRIVE_RENDER_GLINFO_H

#include <string>
#include "opengl.h"

namespace overdrive {
	namespace render {
		bool checkGL(const char* srcFile, int line); // usage - render::checkGL(__FILE__, __LINE__);
		void listGLinfo(bool extensionInfo = false);
	}
}

#endif
