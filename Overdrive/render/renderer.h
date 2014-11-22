#ifndef OVERDRIVE_RENDER_RENDERER_H
#define OVERDRIVE_RENDER_RENDERER_H

#include "opengl.h"

namespace overdrive {
	namespace render {
		/*
			Not a system; this class does the actual rendering. Must be attached to a window or a framebuffer target.
		*/
		class Renderer {
		public:
			Renderer();

		private:
			GLint mDefaultViewport[4];
		};
	}
}

#endif
