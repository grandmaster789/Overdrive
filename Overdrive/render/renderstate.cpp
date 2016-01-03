#include "stdafx.h"
#include "renderstate.h"

namespace {
	void debugOutputCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam
	) {
		(void)source;
		(void)type;
		(void)id;
		(void)severity;
		(void)length;
		(void)message;
		(void)userParam;
	}
}

namespace overdrive {
	namespace render {
		RenderState::RenderState() {
		}

		void RenderState::setDebugOutput(bool enabled, bool synchronous) {
			if (enabled) {
				glEnable(GL_DEBUG_OUTPUT);
			}
			else
				glDisable(GL_DEBUG_OUTPUT);

			if (synchronous)
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			else
				glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}

		void RenderState::clear(eClearOptions buffers) {
			glClear(static_cast<GLbitfield>(buffers));
		}

		void RenderState::setClearColor(const glm::vec4& color) {
			glClearColor(
				color.r,
				color.g,
				color.b,
				color.a
			);
		}
		void RenderState::setClearColor(float r, float g, float b, float a) {
			glClearColor(r, g, b, a);
		}
		void RenderState::setClearDepth(double depth) {
			glClearDepth(depth);
		}

		void RenderState::setClearStencil(int index) {
			glClearStencil(index);
		}

		glm::vec4 RenderState::getClearColor() const {
			float color[4];

			glGetFloatv(GL_COLOR_CLEAR_VALUE, color);

			return glm::vec4(
				color[0], 
				color[1], 
				color[2], 
				color[3]
			);
		}

		double RenderState::getClearDepth() const {
			double result;

			glGetDoublev(GL_DEPTH_CLEAR_VALUE, &result);

			return result;
		}

		int RenderState::getClearStencil() const {
			int result;

			glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &result);

			return result;
		}

		std::ostream& operator << (std::ostream& os, const eClearOptions& options) {
			switch (options) {
			case eClearOptions::NONE: os << "none"; break;
			case eClearOptions::COLOR: os << "color"; break;
			case eClearOptions::DEPTH: os << "depth"; break;
			case eClearOptions::STENCIL: os << "stencil"; break;
			case eClearOptions::COLOR_DEPTH: os << "color | depth"; break;
			case eClearOptions::COLOR_STENCIL: os << "color | stencil"; break;
			case eClearOptions::DEPTH_STENCIL: os << "depth | stencil"; break;
			case eClearOptions::ALL: os << "color | depth | stencil"; break;
			default:
				os << "unknown clear option: " << static_cast<std::underlying_type<eClearOptions>::type>(options);
			}

			return os;
		}
	}
}