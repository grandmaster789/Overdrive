#ifndef OVERDRIVE_RENDER_RENDERBUFFER_H
#define OVERDRIVE_RENDER_RENDERBUFFER_H

#include "opengl.h"
#include "render/texture.h"

namespace overdrive {
	namespace render {
		class RenderBuffer {
		public:
			RenderBuffer();
			RenderBuffer(
				unsigned int width, 
				unsigned int height, 
				eInternalFormat internalFormat
			);
			RenderBuffer(const RenderBuffer&) = delete;
			~RenderBuffer();

			RenderBuffer& operator=  (const RenderBuffer&) = delete;

			void bind();

			void setStorage(
				unsigned int width,
				unsigned int height,
				eInternalFormat internalFormat
			);

			GLuint getHandle() const;

		private:
			GLuint mHandle;
		};
	}
}

#endif
