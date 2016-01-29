#include "overdrive.h"

#include "render/renderstate.h"
#include "render/shaderprogram.h"
#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"
#include "render/vertexarray.h"
#include "video/video.h"
#include "scene/camera.h"
#include "render/shape_cube.h"
#include "render/texture2d.h"

#include <iostream>
#include <boost/math/constants/constants.hpp>

#include "util/csv.h"

using namespace overdrive;

class Test :
	public app::Application,
	public MessageHandler<input::Keyboard::OnKeyPress>
{
public:
	int counter = 0;
	render::RenderState mRenderState;
	std::unique_ptr<render::Texture2D> mTexture;
	render::ShaderProgram mProgram;
	scene::Camera mCamera;

	float mCameraPitch = 0.0f;
	float mCameraYaw = 0.0f;

	std::unique_ptr<render::shape::Cube> mCube;

	Test():
		Application("Test")
	{
		addDependency("Video");
	}

	virtual void initialize() override {
		System::initialize();
		
		mRenderState.setClearColor(0.2f, 0.2f, 0.0f);
		mRenderState.enable(render::eRenderOptions::DEPTH_TEST);

		const char* vertex_shader = R"(
			#version 400
			uniform mat4 inModelMatrix;
			uniform mat4 inViewMatrix;
			uniform mat4 inProjectionMatrix;

			in vec3 inVertexPosition;
			in vec3 inVertexNormal;
			in vec2 inTexCoord;
			
			out vec4		vtxColor;
			smooth out vec2 vtxTexCoord;

						void main() {
				gl_Position = 
					inProjectionMatrix * 
					inViewMatrix * 
					inModelMatrix * 
					vec4(inVertexPosition, 1.0);

				vec3 rescaled_normal = (inVertexNormal + vec3(1.0f, 1.0f, 1.0f)) * 0.5f;

				vtxColor = vec4(rescaled_normal, 1.0f);
				vtxTexCoord = inTexCoord;
			}
		)";

		const char* fragment_shader = R"(
			#version 400
			uniform sampler2D textureMap;

			in vec4 vtxColor;
			in vec2 vtxTexCoord;

			out vec4 outColor;
			
			void main() {
				//outColor = vtxColor;
				//outColor = vec4(vtxTexCoord.x, vtxTexCoord.y, 0.0f, 1.0f);
				outColor = texture(textureMap, vtxTexCoord) * vtxColor;
			}
		)";

		mProgram.attachShader(vertex_shader, render::eShaderType::VERTEX);
		mProgram.attachShader(fragment_shader, render::eShaderType::FRAGMENT);

		mProgram.link();
		mProgram.bind();

		mCamera.setPosition(0.0f, 0.0f, 1.0f);
		mCamera.setProjection(boost::math::float_constants::pi * 0.25f, 1.0f, 0.1f, 100.0f);

		auto mainWindow = mEngine->get<Video>()->getMainWindow();

		mCamera.setViewportWindow(mainWindow);
		mainWindow->getMouse()->setCursorState(input::Mouse::eCursorState::DISABLED); // hide that cursor

		mCube = std::make_unique<render::shape::Cube>(1.0f);

		mTexture = std::make_unique<render::Texture2D>("assets/image/test_pattern_001.png");
	}

	virtual void update() override {
		auto kbd = mEngine->get<Input>()->getKeyboardList().front(); // should mainKeyboard be a thing?
		auto mouse = mEngine->get<Input>()->getMouseList().front(); // should mainMouse be a thing?

		// [WASD keyboard controls]
		{
			auto cameraAxes = mCamera.getDirections();

			glm::vec3 deltaPosition(0.0f, 0.0f, 0.0f);

			if ((*kbd)['W'])
				deltaPosition -= cameraAxes.mForward;
			if ((*kbd)['S'])
				deltaPosition += cameraAxes.mForward;
			if ((*kbd)['A'])
				deltaPosition -= cameraAxes.mRight;
			if ((*kbd)['D'])
				deltaPosition += cameraAxes.mRight;
			if ((*kbd)['Q'])
				deltaPosition += cameraAxes.mUp;
			if ((*kbd)['Z'])
				deltaPosition -= cameraAxes.mUp;

			if ((*kbd)[GLFW_KEY_LEFT_SHIFT])
				deltaPosition *= 5;

			float elapsedSeconds = mEngine->getClock().deltaFrame().count() * 0.001f; // deltaframe is in milliseconds
			float movementSpeed = 1.0f * elapsedSeconds;
			deltaPosition *= movementSpeed;

			mCamera.setPosition(mCamera.getPosition() + deltaPosition);
		}

		// [Mouselook] ~ yaw/pitch based
		{
			static std::pair<double, double> previousMousePosition = std::make_pair(0.0, 0.0);
			auto currentPos = mouse->getPosition();

			if ((previousMousePosition.first == 0.0) && (previousMousePosition.second == 0.0))
				previousMousePosition = currentPos;
			
			float dx = 0.01f * static_cast<float>(currentPos.first - previousMousePosition.first);
			float dy = 0.01f * static_cast<float>(currentPos.second - previousMousePosition.second);
			previousMousePosition = currentPos;

			mCameraYaw += dx;
			mCameraPitch += dy;

			auto yawQ = glm::rotate(mCameraYaw, glm::vec3(0.0f, 1.0f, 0.0f));
			auto pitchQ = glm::rotate(mCameraPitch, glm::vec3(1.0f, 0.0f, 0.0f));

			glm::quat q = glm::quat_cast(pitchQ * yawQ);

			mCamera.setOrientation(q);
		}

		mCamera.update();

		mRenderState.clear();
		mProgram.setUniform("inViewMatrix", mCamera.getView());
		mProgram.setUniform("inProjectionMatrix", mCamera.getProjection());
		
		mTexture->bind(0);
		mProgram.setUniform("textureMap", 0);
		
		// draw the cube at 100 different locations
		for (int i = 0; i < 10; ++i)
			for (int j = 0; j < 10; ++j) {
				mProgram.setUniform("inModelMatrix", glm::translate(glm::vec3(2 * (i - 5), 0, 2 * (j - 5))));

				mCube->draw();
			}
	}

	virtual void shutdown() override {
		System::shutdown();
	}

	void operator()(const input::Keyboard::OnKeyPress& kp) {
		switch (kp.mKey) {
		case GLFW_KEY_ESCAPE:
			mChannel.broadcast(core::Engine::OnStop());
			break;

		case GLFW_KEY_F1:
			gLog << mProgram;
			break;
		}
	}
};

int main() {
	gLog << "Starting Overdrive Assault";
		
	core::Engine engine;
	engine.add(new overdrive::Video);
	engine.add(new overdrive::Input);
	engine.setApplication(new Test);

	engine.run();

	gLog << "Done";
}