#pragma once

#include "../opengl.h"
#include "texture2D.h"
#include "texture2DMultisample.h"

namespace overdrive {
	namespace render {
		// [NOTE] it appears that attaching 1d or 3d textures should also be possible, need to look into that
		// http://docs.gl/gl4/glBindFramebuffer
		// http://docs.gl/gl4/glDrawBuffers
		class FrameBuffer {
		public:
			FrameBuffer();
			~FrameBuffer();

			FrameBuffer(const FrameBuffer&) = delete;
			FrameBuffer(FrameBuffer&&) = delete;
			FrameBuffer& operator = (const FrameBuffer&) = delete;
			FrameBuffer& operator = (FrameBuffer&&) = delete;

			GLuint getHandle() const;

			void attachColor(GLuint unit, const Texture2D& texture);
			void attachColor(GLuint unit, const Texture2DMultisample& texture);
			void attachDepth(const Texture2D& texture);
			void attachDepth(const Texture2DMultisample& texture);
			// atttach stencil w/o depth?
			// attach renderbuffer?
			
			void bind();
			void unbind();

			void drawAttachment(GLuint index_0);
			void drawAttachment(GLuint index_0, GLuint index_1);
			void drawAttachment(GLuint index_0, GLuint index_1, GLuint index_2);
			void drawAttachment(GLuint index_0, GLuint index_1, GLuint index_2, GLuint index_3);

		private:
			GLuint mHandle;
		};
	}
}
