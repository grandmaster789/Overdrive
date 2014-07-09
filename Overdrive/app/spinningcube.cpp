#include "app/spinningcube.h"
#include "core/engine.h"
#include "video/video.h"
#include "math/conversions.h"
#include "render/glinfo.h"

namespace overdrive {
	namespace app {
		SpinningCube::SpinningCube():
			Application("Spinning Cube")
		{
		}

		bool SpinningCube::initialize() {
			System::initialize();

			render::listGLinfo();

			glClearColor(0.1f, 0.0f, 0.0f, 0.0f);
			glEnable(GL_DEPTH_TEST);

			mModel = glm::mat4(1.0f);
			mView = glm::lookAt(
				glm::vec3(0.0f, 0.0f, 2.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
			mProjection = glm::mat4(1.0f); // identity projection

			auto video = mEngine->get<video::Video>();
			auto windows = video->getWindows();
			
			assert(!windows.empty());

			int primaryWidth = windows.front().getWidth();
			int primaryHeight = windows.front().getHeight();

			mProjection = glm::perspective(
				math::toRadians(70.0f), 
				static_cast<float>(primaryWidth) / static_cast<float>(primaryHeight), 
				0.1f, 
				100.0f
			); // 70'fov perspective projection

			glViewport(0, 0, primaryWidth, primaryHeight);

			mAngle = 0.01f;

			mShaderProgram.compileFile("shaders/diffuse.vs", render::VERTEX);
			mShaderProgram.compileFile("shaders/diffuse.fs", render::FRAGMENT);
			mShaderProgram.link();

			mShaderProgram.use();

			mShaderProgram.set("LightDiffuse", 1.0f, 1.0f, 1.0f);
			mShaderProgram.set("MaterialDiffuse", 0.9f, 0.5f, 0.3f);
			mShaderProgram.set("LightPosition", mView * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f));

			mCube = std::make_unique<render::shape::Cube>();
			mTorus = std::make_unique<render::shape::Torus>();
			mPlane = std::make_unique<render::shape::Plane>();

			mEngine->updateSystem(this, true, false);

			return true;
		}

		void SpinningCube::update() {
			mModel *= glm::rotate(mAngle, glm::vec3(0.1f, 1.0f, 0.0f));

			glm::mat4 modelView = mView * mModel;
			
			mShaderProgram.set("ModelViewMatrix", modelView);
			mShaderProgram.set("NormalMatrix", glm::mat3(
				glm::vec3(modelView[0]),
				glm::vec3(modelView[1]),
				glm::vec3(modelView[2])
			));
			mShaderProgram.set("ModelViewProjection", mProjection * modelView);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			//mCube->draw();
			mTorus->draw();
			//mPlane->draw();
		}

		void SpinningCube::shutdown() {
			System::shutdown();
		}

		void SpinningCube::operator()(const input::Keyboard::OnKeyPress& kp) {
			if (kp.mKey == GLFW_KEY_ESCAPE)
				mChannel.broadcast(core::Engine::OnStop());
		}
	}
}