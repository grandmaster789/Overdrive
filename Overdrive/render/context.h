#ifndef OVERDRIVE_RENDER_CONTEXT_H
#define OVERDRIVE_RENDER_CONTEXT_H

#include "util/color.h"
#include "render/vertexarray.h"
#include "render/shaderprogram.h"
#include "render/framebuffer.h"

namespace overdrive {
	namespace render {
		enum class eBufferLayer {
			COLOR,
			DEPTH,
			STENCIL,

			COLOR_AND_DEPTH,
			COLOR_AND_STENCIL,
			DEPTH_AND_STENCIL,

			ALL
		};

		enum class eDrawingMode: GLint {
			TRIANGLES = GL_TRIANGLES,
			LINES = GL_LINES,
			POINTS = GL_POINTS
		};

		enum class eCullFace: GLenum {
			FRONT = GL_FRONT,
			BACK = GL_BACK,
			FRONT_AND_BACK = GL_FRONT_AND_BACK
		};

		enum class eTestFunction: GLenum {
			NEVER = GL_NEVER,
			ALWAYS = GL_ALWAYS,
			LESS = GL_LESS,
			LESS_EQUAL = GL_LEQUAL,
			GREATER = GL_GREATER,
			GREATER_EQUAL = GL_GEQUAL,
			EQUAL = GL_EQUAL,
			NOT_EQUAL = GL_NOTEQUAL
		};

		enum class eStencilOperation: GLenum {
			KEEP = GL_KEEP,
			ZERO = GL_ZERO,
			REPLACE = GL_REPLACE,
			INCREASE = GL_INCR,
			INCREASE_WRAP = GL_INCR_WRAP,
			DECREASE = GL_DECR,
			DECREASE_WRAP = GL_DECR_WRAP,
			INVERT = GL_INVERT
		};

		class Context {
		public:
			void activate();

			void setVSync(bool enable);

			void setCullFace(bool enable, eCullFace cull = eCullFace::BACK);
			
			void setRasterizerDiscard(bool enable); // toggles rasterization (disabling the pixel shader stage)

			void setDepthTest(bool enable);
			void setDepthMask(bool writeEnabled);
			void setDepthFn(eTestFunction fn);

			void setStencilTest(bool enable);
			void setStencilMask(bool writeEnabled);
			void setStencilFn(
				eTestFunction fn, 
				int reference, 
				unsigned int mask = ~0
			);
			void setStencilOp(
				eStencilOperation onFail, 
				eStencilOperation onZFail, 
				eStencilOperation onPass
			);

			void setClearColor(const util::Color& c);
			void clear(eBufferLayer layers = eBufferLayer::COLOR_AND_DEPTH);

			void use(const ShaderProgram& program);

			void draw(const VertexArray& vao, eDrawingMode primitiveType = eDrawingMode::TRIANGLES);

			void bindFrameBuffer(); // restores default framebuffer target
			void bindFrameBuffer(const FrameBuffer& framebuffer);

		private:
			GLint mViewport[4];
		};
	}
}

#endif
