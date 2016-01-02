#pragma once

#include "../opengl.h"
#include <ostream>

namespace overdrive {
	namespace video {
		// http://www.glfw.org/docs/latest/structGLFWvidmode.html
		struct VideoMode {
			VideoMode();
			VideoMode(const GLFWvidmode& source);
			VideoMode(const GLFWvidmode* source);

			int mWidth;
			int mHeight;
			
			int mRedBits;
			int mGreenBits;
			int mBlueBits;
			
			int mRefreshRate;
		};
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::video::VideoMode& vm);