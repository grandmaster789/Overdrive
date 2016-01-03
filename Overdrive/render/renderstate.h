#pragma once

#include "../opengl.h"
#include <ostream>

namespace overdrive {
	namespace render {
		enum class eClearOptions: GLbitfield {
			NONE			= 0,
			COLOR			= GL_COLOR_BUFFER_BIT,
			DEPTH			= GL_DEPTH_BUFFER_BIT,
			STENCIL			= GL_STENCIL_BUFFER_BIT,
			COLOR_DEPTH		= (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),
			COLOR_STENCIL	= (GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT),
			DEPTH_STENCIL	= (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT),
			ALL				= (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)
		};

		// [NOTE] As I make use of various openGL state functions, this'll gradually get completed
		// http://docs.gl/gl4/glEnable
		// http://docs.gl/gl4/glClear
		class RenderState {
		public:
			RenderState();

			void clear(eClearOptions buffers = eClearOptions::ALL);
			void setClearColor(const glm::vec4& color);
			void setClearColor(float r, float g, float b, float a = 1.0f);
			void setClearDepth(double depth);
			void setClearStencil(int index);

			glm::vec4 getClearColor() const;
			double getClearDepth() const;
			int getClearStencil() const;

		private:

		};

		std::ostream& operator << (std::ostream& os, const eClearOptions& options);
	}
}
