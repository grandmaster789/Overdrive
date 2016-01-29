#pragma once

#include "../opengl.h"
#include "texture.h"
#include "texture2D.h"

namespace overdrive {
	namespace render {
		enum class eFramebufferBind : GLenum  {
			READ		= GL_READ_FRAMEBUFFER,
			WRITE		= GL_DRAW_FRAMEBUFFER,
			DRAW		= GL_DRAW_FRAMEBUFFER,
			READ_WRITE	= GL_FRAMEBUFFER
		};

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
			void attachDepth(const Texture2D& texture);

			// atttach stencil w/o depth?
			// attach renderbuffer?
			
			void bind(eFramebufferBind options = eFramebufferBind::READ_WRITE);
			void unbind(eFramebufferBind options = eFramebufferBind::READ_WRITE);

			void drawAttachment(GLuint index_0);
			void drawAttachment(GLuint index_0, GLuint index_1);
			void drawAttachment(GLuint index_0, GLuint index_1, GLuint index_2);
			void drawAttachment(GLuint index_0, GLuint index_1, GLuint index_2, GLuint index_3);

		private:
			GLuint mHandle;
		};

		std::ostream& operator << (std::ostream& os, const eFramebufferBind& value);
	}
}
