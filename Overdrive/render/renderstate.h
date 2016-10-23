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

		enum class eBlendFunctionFactor : GLenum {
			ZERO						= GL_ZERO, 
			ONE							= GL_ONE, 
			SRC_COLOR					= GL_SRC_COLOR,
			ONE_MINUS_SRC_COLOR			= GL_ONE_MINUS_SRC_COLOR, 
			DST_COLOR					= GL_DST_COLOR, 
			ONE_MINUS_DST_COLOR			= GL_ONE_MINUS_DST_COLOR, 
			SRC_ALPHA					= GL_SRC_ALPHA, 
			ONE_MINUS_SRC_ALPHA			= GL_ONE_MINUS_SRC_ALPHA, 
			DST_ALPHA					= GL_DST_ALPHA, 
			ONE_MINUS_DST_ALPHA			= GL_ONE_MINUS_DST_ALPHA,
			CONSTANT_COLOR				= GL_CONSTANT_COLOR, 
			ONE_MINUS_CONSTANT_COLOR	= GL_ONE_MINUS_CONSTANT_COLOR, 
			CONSTANT_ALPHA				= GL_CONSTANT_ALPHA,
			ONE_MINUS_CONSTANT_ALPHA	= GL_ONE_MINUS_CONSTANT_ALPHA
		};

		struct BlendFunction {
			eBlendFunctionFactor mSourceFactor;
			eBlendFunctionFactor mDestinationFactor;
		};

		enum class eCullMode : GLenum {
			FRONT			= GL_FRONT,
			BACK			= GL_BACK,
			FRONT_AND_BACK	= GL_FRONT_AND_BACK
		};

		// [NOTE] when doing state sorting, it would be better if this wasn't using GLenum as the underlying type...
		enum class eRenderOptions: GLenum {
			DEPTH_TEST		= GL_DEPTH_TEST,
			CULL_FACE		= GL_CULL_FACE,
		};

		// [NOTE] As I make use of various openGL state functions, this'll gradually get completed
		// [TODO] implement operators for state comparision/sorting
		// http://docs.gl/gl4/glEnable
		// http://docs.gl/gl4/glIsEnabled
		// http://docs.gl/gl4/glClear
		class RenderState {
		public:
			RenderState();

			static void clear(eClearOptions buffers = eClearOptions::ALL);
			
			void setClearColor(const glm::vec4& color);
			void setClearColor(float r, float g, float b, float a = 1.0f);
			void setClearDepth(double depth);
			void setClearStencil(int index);
			void setCullMode(eCullMode mode);

			const glm::vec4& getClearColor() const;
			double getClearDepth() const;
			int getClearStencil() const;
			eCullMode getCullMode() const;

			void enable(eRenderOptions option);
			void disable(eRenderOptions option);
			bool isEnabled(eRenderOptions option) const;

			void apply();
			static RenderState getActiveState();

		private:
			// [NOTE] these defaults are specified by openGL
			bool mDepthTestEnabled	= false;
			bool mCullFaceEnabled	= false;

			glm::vec4 mClearColor	= glm::vec4(0, 0, 0, 0);
			double mClearDepth		= 1.0;
			int mClearStencil		= 0;
			eCullMode mCullMode		= eCullMode::BACK;
		};

		std::ostream& operator << (std::ostream& os, const eClearOptions& option);
		std::ostream& operator << (std::ostream& os, const eRenderOptions& option);
		std::ostream& operator << (std::ostream& os, const eCullMode& mode);
		std::ostream& operator << (std::ostream& os, const eBlendFunctionFactor& factor);
		std::ostream& operator << (std::ostream& os, const BlendFunction& fn);
		std::ostream& operator << (std::ostream& os, const RenderState& state);
	}
}
