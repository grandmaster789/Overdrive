#ifndef OVERDRIVE_APP_SPINNINGCUBE_H
#define OVERDRIVE_APP_SPINNINGCUBE_H

#include "app/application.h"
#include "render/shaderprogram.h"
#include "render/shape/cube.h"
#include "input/keyboard.h"

namespace overdrive {
	namespace app {
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
		};
	}
}

#endif
