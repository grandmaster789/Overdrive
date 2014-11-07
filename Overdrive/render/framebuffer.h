#ifndef OVERDRIVE_RENDER_FRAMEBUFFER_H
#define OVERDRIVE_RENDER_FRAMEBUFFER_H

#include "opengl.h"
#include "render/texture.h"

namespace overdrive {
	namespace render {
		class FrameBuffer {
		public:
			FrameBuffer(
				unsigned int width, 
				unsigned int height, 
				unsigned char colorBits = 32, 
				unsigned char depthBits = 24
			);
			~FrameBuffer();

			GLuint getHandle() const;

			const Texture& getColorBuffer() const;
			const Texture& getDepthBuffer() const;

		private:
			GLuint mHandle;

			Texture mColorBuffer;
			Texture mDepthBuffer;
		};
	}
}

#endif
