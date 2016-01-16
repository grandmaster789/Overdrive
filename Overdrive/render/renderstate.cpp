#include "stdafx.h"
#include "renderstate.h"

namespace overdrive {
	namespace render {
		RenderState::RenderState() {
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

		void RenderState::enable(eRenderOptions option) {
			glEnable(static_cast<GLenum>(option));
		}

		void RenderState::disable(eRenderOptions option) {
			glDisable(static_cast<GLenum>(option));
		}

		bool RenderState::isEnabled(eRenderOptions option) const {
			return (glIsEnabled(static_cast<GLenum>(option)) != GL_FALSE);
		}

		std::ostream& operator << (std::ostream& os, const eClearOptions& option) {
			switch (option) {
			case eClearOptions::NONE: os << "none"; break;
			case eClearOptions::COLOR: os << "color"; break;
			case eClearOptions::DEPTH: os << "depth"; break;
			case eClearOptions::STENCIL: os << "stencil"; break;
			case eClearOptions::COLOR_DEPTH: os << "color | depth"; break;
			case eClearOptions::COLOR_STENCIL: os << "color | stencil"; break;
			case eClearOptions::DEPTH_STENCIL: os << "depth | stencil"; break;
			case eClearOptions::ALL: os << "color | depth | stencil"; break;
			default:
				os << "unknown clear option: " << static_cast<std::underlying_type<eClearOptions>::type>(option);
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eRenderOptions& option) {
			switch (option) {
			case eRenderOptions::DEPTH_TEST: os << "Depth Testing"; break;
			default:
				os << "Unknown render state option: " << static_cast<std::underlying_type_t<eRenderOptions>>(option);
			}

			return os;
		}
	}
}