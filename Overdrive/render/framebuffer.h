#ifndef OVERDRIVE_RENDER_FRAMEBUFFER_H
#define OVERDRIVE_RENDER_FRAMEBUFFER_H

#include "opengl.h"
#include "render/texture2D.h"

namespace overdrive {
	namespace render {
		class FrameBuffer {
		public:
			/*
			 The color bits *must* be either 24 or 32 bits
			 The depth bits *must* be 8/16/24/32 bits
			 
			 other bit specifications are not supported and will result in FrameBufferException
			 [note to self] perhaps make the depth buffer optional?
			 */
			FrameBuffer(
				unsigned int width, 
				unsigned int height, 
				unsigned char colorBits = 32, 
				unsigned char depthBits = 24
			);
			~FrameBuffer();

			void bind();
			void unbind();

			GLuint getHandle() const;

			const Texture2D& getColorBuffer() const;
			const Texture2D& getDepthBuffer() const;

		private:
			GLuint mHandle;

			Texture2D mColorBuffer;
			Texture2D mDepthBuffer;

			struct BufferGuard {
				BufferGuard();
				~BufferGuard();

				GLint mOriginal;
			};
		};

		class FrameBufferException: public std::exception {
		public:
			virtual const char* what() const throw();
		};

		class FrameBufferBitsException: public std::exception {
		public:
			FrameBufferBitsException(unsigned int bits);

			virtual const char* what() const throw();

		private:
			unsigned int mNumBitsRequested;
		};
	}
}

#endif
