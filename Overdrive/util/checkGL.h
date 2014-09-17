#ifndef OVERDRIVE_UTIL_CHECKGL_H
#define OVERDRIVE_UTIL_CHECKGL_H

namespace overdrive {
	namespace util {
		bool checkGL(const char* file, int line);
	}
}

#define CHECK_GL_STATE overdrive::util::checkGL(__FILE__, __LINE__)

#endif
