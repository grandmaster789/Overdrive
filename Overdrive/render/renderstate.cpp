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
			mClearColor = color;
		}

		void RenderState::setClearColor(float r, float g, float b, float a) {
			mClearColor = glm::vec4(r, g, b, a);
		}
		void RenderState::setClearDepth(double depth) {
			mClearDepth = depth;
		}

		void RenderState::setClearStencil(int index) {
			mClearStencil = index;
		}

		void RenderState::setCullMode(eCullMode mode) {
			mCullMode = mode;
		}

		const glm::vec4& RenderState::getClearColor() const {
			return mClearColor;
		}

		double RenderState::getClearDepth() const {
			return mClearDepth;
		}

		int RenderState::getClearStencil() const {
			return mClearStencil;
		}

		eCullMode RenderState::getCullMode() const {
			return mCullMode;
		}

		void RenderState::enable(eRenderOptions option) {
			switch (option) {
			case eRenderOptions::DEPTH_TEST: mDepthTestEnabled = true; break;
			case eRenderOptions::CULL_FACE:	 mCullFaceEnabled = true; break;
			default:
				break;
			}
		}

		void RenderState::disable(eRenderOptions option) {
			switch (option) {
			case eRenderOptions::DEPTH_TEST: mDepthTestEnabled = false; break;
			case eRenderOptions::CULL_FACE:	 mCullFaceEnabled = false; break;
			}
		}

		bool RenderState::isEnabled(eRenderOptions option) const {
			switch (option) {
			case eRenderOptions::DEPTH_TEST: return mDepthTestEnabled;
			case eRenderOptions::CULL_FACE: return mCullFaceEnabled;
			default:
				return false;
			}
		}

		namespace {
			void setRenderOption(eRenderOptions option, bool enabled, bool& current) {
				if (enabled != current) {
					if (enabled)
						glEnable(static_cast<GLenum>(option));
					else
						glDisable(static_cast<GLenum>(option));

					current = enabled;
				}
			}
		}

		void RenderState::apply() {
			static RenderState currentState;

			setRenderOption(eRenderOptions::DEPTH_TEST, mDepthTestEnabled, currentState.mDepthTestEnabled);
			setRenderOption(eRenderOptions::CULL_FACE, mCullFaceEnabled, currentState.mCullFaceEnabled);

			if (mClearColor != currentState.mClearColor) {
				glClearColor(
					mClearColor.r, 
					mClearColor.g, 
					mClearColor.b, 
					mClearColor.a
				);
				currentState.mClearColor = mClearColor;
			}

			if (mClearDepth != currentState.mClearDepth) {
				glClearDepth(mClearDepth);
				currentState.mClearDepth = mClearDepth;
			}

			if (mClearStencil != currentState.mClearStencil) {
				glClearStencil(mClearStencil);
				currentState.mClearStencil = mClearStencil;
			}

			if (mCullMode != currentState.mCullMode) {
				glCullFace(static_cast<GLenum>(mCullMode));
				currentState.mCullMode = mCullMode;
			}
		}

		RenderState RenderState::getActiveState() {
			RenderState result;

			result.mDepthTestEnabled = (glIsEnabled(GL_DEPTH_TEST) == GL_TRUE);
			result.mCullFaceEnabled = (glIsEnabled(GL_CULL_FACE) == GL_TRUE);

			{
				float color[4];
				
				glGetFloatv(GL_COLOR_CLEAR_VALUE, color);
				
				result.mClearColor = glm::vec4(
					color[0], 
					color[1], 
					color[2], 
					color[3]
				);
			}

			glGetDoublev(GL_DEPTH_CLEAR_VALUE, &result.mClearDepth);
			glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &result.mClearStencil);

			{
				int mode;
				glGetIntegerv(GL_CULL_FACE_MODE, &mode);

				switch (mode) {
				case GL_BACK: 
					result.mCullMode = eCullMode::BACK; 
					break;

				case GL_FRONT: 
					result.mCullMode = eCullMode::FRONT; 
					break;

				case GL_FRONT_AND_BACK:
					result.mCullMode = eCullMode::FRONT_AND_BACK;
					break;
				}
			}

			return result;
		}

		std::ostream& operator << (std::ostream& os, const eClearOptions& option) {
			switch (option) {
			case eClearOptions::NONE:			os << "none";						break;
			case eClearOptions::COLOR:			os << "color";						break;
			case eClearOptions::DEPTH:			os << "depth";						break;
			case eClearOptions::STENCIL:		os << "stencil";					break;
			case eClearOptions::COLOR_DEPTH:	os << "color | depth";				break;
			case eClearOptions::COLOR_STENCIL:	os << "color | stencil";			break;
			case eClearOptions::DEPTH_STENCIL:	os << "depth | stencil";			break;
			case eClearOptions::ALL:			os << "color | depth | stencil";	break;
			default:
				os << "unknown clear option: " << static_cast<std::underlying_type<eClearOptions>::type>(option);
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eRenderOptions& option) {
			switch (option) {
			case eRenderOptions::DEPTH_TEST:	os << "Depth Testing";				break;
			case eRenderOptions::CULL_FACE:		os << "Backface culling";			break;

			default:
				os 
					<< "Unknown render state option: " 
					<< static_cast<std::underlying_type_t<eRenderOptions>>(option);
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eCullMode& mode) {
			switch (mode) {
			case eCullMode::BACK:			os << "back";			break;
			case eCullMode::FRONT:			os << "front";			break;
			case eCullMode::FRONT_AND_BACK:	os << "front and back"; break;
			}
			return os;
		}

		std::ostream& operator << (std::ostream& os, const RenderState& state) {
			os
				<< "RenderState:\n"
				<< "\tDepth testing: " << std::boolalpha << state.isEnabled(eRenderOptions::DEPTH_TEST) << "\n"
				<< "\tBackface culling: " << std::boolalpha << state.isEnabled(eRenderOptions::CULL_FACE) << "\n"
				<< "\t-------------------------------------------\n"
				<< "\tClear color: " << state.getClearColor() << "\n"
				<< "\tClear depth: " << state.getClearDepth() << "\n"
				<< "\tClear stencil: " << state.getClearStencil() << "\n"
				<< "\tBackface culling mode: " << state.getCullMode() << "\n";

			return os;
		}
	}
}