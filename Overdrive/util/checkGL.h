#ifndef OVERDRIVE_UTIL_CHECKGL_H
#define OVERDRIVE_UTIL_CHECKGL_H

namespace overdrive {
	namespace util {
		// in release mode, this doesn't do anything
		bool checkGL(const char* file, int line);
	}
}

#ifdef _DEBUG
	#define CHECK_GL_STATE overdrive::util::checkGL(__FILE__, __LINE__)
#else
	#define CHECK_GL_STATE
#endif

#endif
