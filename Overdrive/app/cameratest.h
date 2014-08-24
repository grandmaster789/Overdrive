#ifndef OVERDRIVE_APP_CAMERATEST_H
#define OVERDRIVE_APP_CAMERATEST_H

#include "app/application.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "render/shape/cube.h"
#include "render/shaderprogram.h"
#include "render/camera/freeCamera.h"

namespace overdrive {
	namespace app {
		class CameraTest:
			public Application,
			public core::MessageHandler<input::Keyboard::OnKeyPress>,
			public core::MessageHandler<input::Mouse::OnMove>
		{
		public:
			CameraTest();

			virtual bool initialize() override;
			virtual void update() override;
			virtual void shutdown() override;

			virtual void operator()(const input::Keyboard::OnKeyPress& kp) override;
			virtual void operator()(const input::Mouse::OnMove& mm) override;

		private:
			glm::mat4 mModel;
			
			float mAngle;

			render::ShaderProgram mShaderProgram;

			render::camera::FreeCamera mCamera;

			std::unique_ptr<render::shape::Cube> mCube;
		};
	}
}

#endif
