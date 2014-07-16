#ifndef OVERDRIVE_APP_SPINNINGCUBE_H
#define OVERDRIVE_APP_SPINNINGCUBE_H

#include "app/application.h"
#include "render/shaderprogram.h"
#include "render/shape/cube.h"
#include "render/shape/torus.h"
#include "render/shape/plane.h"
#include "render/shape/grid.h"
#include "input/keyboard.h"

namespace overdrive {
	namespace app {
		/*
			Really basic application, showing a spinning cube and shutting down when escape is pressed
			Minimal requirements:
				- System features (init/update/shutdown)
				- Channel features (MessageHandler, onKeyPress)
				- ShaderProgram (diffuse shaders)
				- render::shape::Cube

			Does not use/require camera's, models, lights, etc.
		 */
		class SpinningCube :
			public Application,
			public core::MessageHandler<input::Keyboard::OnKeyPress>
		{
		public:
			SpinningCube();

			bool initialize();
			void update();
			void shutdown();

			void operator()(const input::Keyboard::OnKeyPress& kp);

		private:
			glm::mat4 mModel;
			glm::mat4 mView;
			glm::mat4 mProjection;

			float mAngle;

			render::ShaderProgram mShaderProgram;

			std::unique_ptr<render::shape::Cube> mCube;
			std::unique_ptr<render::shape::Torus> mTorus;
			std::unique_ptr<render::shape::Plane> mPlane;
			std::unique_ptr<render::shape::Grid> mGrid;
		};
	}
}

#endif