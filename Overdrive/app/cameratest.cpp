#include "app/cameratest.h"
#include "core/engine.h"
#include "video/video.h"

namespace overdrive {
	namespace app {
		CameraTest::CameraTest():
			Application("Camera Test")
		{
		}

		bool CameraTest::initialize() {
			System::initialize();

			glClearColor(0.1f, 0.0f, 0.0f, 0.0f);
			glEnable(GL_DEPTH_TEST);
			
			mModel = glm::mat4(1.0f);
			mAngle = 0.01f;

			auto video = mEngine->get<video::Video>();
			auto windows = video->getWindows();

			assert(!windows.empty());

			int primaryWidth = windows.front().getWidth();
			int primaryHeight = windows.front().getHeight();

			glm::mat4 projection = glm::perspective(
				math::toRadians(70.0f),
				static_cast<float>(primaryWidth) / static_cast<float>(primaryHeight),
				0.1f,
				100.0f
			); // 70'fov perspective projection

			mCamera.setPosition(0.0f, 0.0f, 2.0f);
			mCamera.setProjection(projection);
			mCamera.update();

			mShaderProgram.compileFile("shaders/diffuse.vs", render::VERTEX);
			mShaderProgram.compileFile("shaders/diffuse.fs", render::FRAGMENT);
			mShaderProgram.link();

			mShaderProgram.enable();

			mShaderProgram.set("LightDiffuse", 1.0f, 1.0f, 1.0f);
			mShaderProgram.set("MaterialDiffuse", 0.9f, 0.5f, 0.3f);
			mShaderProgram.set("LightPosition", mCamera.getView() * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f));

			mCube = std::make_unique<render::shape::Cube>();

			mEngine->updateSystem(this, true, false);

			return true;
		}

		void CameraTest::update() {
			mCamera.update();

			mModel *= glm::rotate(mAngle, glm::vec3(0.1f, 1.0f, 0.0f));
			
			glm::mat4 modelView = mCamera.getView() * mModel;

			mShaderProgram.set("ModelViewMatrix", modelView);
			mShaderProgram.set("NormalMatrix", glm::mat3(
				glm::vec3(modelView[0]),
				glm::vec3(modelView[1]),
				glm::vec3(modelView[2])
			));
			mShaderProgram.set("ModelViewProjection", mCamera.getProjection() * modelView);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			mCube->draw();
		}

		void CameraTest::shutdown() {
			System::shutdown();
		}

		void CameraTest::operator()(const input::Keyboard::OnKeyPress& kp) {
			switch (kp.mKey) {
			case GLFW_KEY_ESCAPE:
				mChannel.broadcast(core::Engine::OnStop());
				break;

			case 'W':
			case 'w':
				mCamera.moveForward(1.0f);
				break;

			case 'A':
			case 'a':
				mCamera.moveRight(-1.0f);
				break;

			case 'S':
			case 's':
				mCamera.moveForward(-1.0f);
				break;

			case 'D':
			case 'd':
				mCamera.moveRight(1.0f);
				break;

			default:
				break;
			}
		}

		void CameraTest::operator()(const input::Mouse::OnMove& mm) {
			(void)mm;
		}
	}
}