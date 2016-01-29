#pragma once

#include "../opengl.h"
#include "shaderprogram.h"
#include "framebuffer.h"
#include "renderstate.h"
#include "shape_cube.h"

#include "../video/window.h"
#include "../scene/camera.h"


namespace overdrive {
	namespace render {

		class Renderer
		{
		public:
			Renderer(video::Window* associatedWindow);

			void submit();
			void present();
			
		private:
			video::Window* mWindow;
			RenderState mState;

			// default parameters passed to everything rendered:
			glm::mat4 mView;
			glm::mat4 mProjection;
		};
	}
}
