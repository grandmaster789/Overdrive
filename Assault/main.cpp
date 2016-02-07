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
#include "render/texturecube.h"
#include "render/defaultShaders.h"

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
	render::Texture2D mTexture;
	render::ShaderProgram mProgram;
	
	render::TextureCube mSkyBoxTexture;
	render::ShaderProgram mSkyBoxProgram;
	std::unique_ptr<render::shape::Cube> mSkyBox;

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
			uniform mat4 uModel;
			uniform mat4 uView;
			uniform mat4 uProjection;

			in vec3 aVertexPosition;
			in vec3 aVertexNormal;
			in vec2 aTexCoord;
			
			out vec4		vtxColor;
			smooth out vec2 vtxTexCoord;

			void main() {
				gl_Position = 
					uProjection * 
					uView * 
					uModel * 
					vec4(aVertexPosition, 1.0);

				vec3 rescaled_normal = (aVertexNormal + vec3(1.0f, 1.0f, 1.0f)) * 0.5f;

				vtxColor = vec4(rescaled_normal, 1.0f);
				vtxTexCoord = aTexCoord;
			}
		)";

		const char* fragment_shader = R"(
			#version 400
			uniform sampler2D uTexture;

			in vec4 vtxColor;
			in vec2 vtxTexCoord;

			out vec4 outColor;
			
			void main() {
				//outColor = vtxColor;
				//outColor = vec4(vtxTexCoord.x, vtxTexCoord.y, 0.0f, 1.0f);
				outColor = texture(uTexture, vtxTexCoord) * vtxColor;
			}
		)";

		const char* skybox_vertex_shader = R"(
			#version 400
			uniform mat4 uModel;
			uniform mat4 uView;
			uniform mat4 uProjection;

			in vec3 aVertexPosition;

			smooth out vec3 cubemap_texcoords;

			void main() {
				gl_Position = 
					uProjection * 
					uView * 
					uModel * 
					vec4(aVertexPosition, 1.0);

				cubemap_texcoords = aVertexPosition;
			}
		)";

		const char* skybox_fragment_shader = R"(
			#version 400			

			uniform samplerCube uCubeMap;
			smooth in vec3 cubemap_texcoords;

			out vec4 outColor;
		
			void main() {
				outColor = texture(uCubeMap, cubemap_texcoords);
			}
		)";

		mProgram.attachShader(render::DefaultShader<render::attributes::PositionNormalTexCoord>::getVertexShader(), render::eShaderType::VERTEX);
		mProgram.attachShader(render::DefaultShader<render::attributes::PositionNormalTexCoord>::getFragmentShader(), render::eShaderType::FRAGMENT);
		mProgram.link();
		mProgram.bind();

		mSkyBoxProgram.attachShader(skybox_vertex_shader, render::eShaderType::VERTEX);
		mSkyBoxProgram.attachShader(skybox_fragment_shader, render::eShaderType::FRAGMENT);
		mSkyBoxProgram.link();
		mSkyBoxProgram.bind();

		mCamera.setPosition(0.0f, 0.0f, 1.0f);
		mCamera.setProjection(boost::math::float_constants::pi * 0.25f, 1.0f, 0.1f, 100.0f);

		auto mainWindow = mEngine->get<Video>()->getMainWindow();

		mCamera.setViewportWindow(mainWindow);
		mainWindow->getMouse()->setCursorState(input::Mouse::eCursorState::DISABLED); // hide that cursor

		mCube = std::make_unique<render::shape::Cube>(1.0f);
		mSkyBox = std::make_unique<render::shape::Cube>(
			115.0f // far plane should end up at the center of the skybox cube
		); // [NOTE] the skybox cube should be smaller than the far clipping plane; 

		mTexture = render::loadTexture2D("assets/image/test_pattern_001.png");
		
		mSkyBoxTexture = render::loadTextureCube(
			"assets/image/skybox_px.png",
			"assets/image/skybox_py.png",
			"assets/image/skybox_pz.png",
			"assets/image/skybox_nx.png",
			"assets/image/skybox_ny.png",
			"assets/image/skybox_nz.png"
		);		
		
		//mSkyBoxTexture = render::loadTextureCube("assets/image/skybox_px.png");
		gLog << mProgram;
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

		// render that skybox
		mSkyBoxProgram.bind();
		mSkyBoxProgram.setUniform("uView", mCamera.getView());
		mSkyBoxProgram.setUniform("uProjection", mCamera.getProjection());
		mSkyBoxProgram.setUniform("uModel", glm::translate(mCamera.getPosition()));
		
		mSkyBoxTexture.bind(0);
		mSkyBoxProgram.setUniform("uCubeMap", 0);
		
		mSkyBox->draw();

		// render the cubes
		mProgram.bind();
		mProgram.setUniform("uView", mCamera.getView());
		mProgram.setUniform("uProjection", mCamera.getProjection());

		mProgram.setUniform("uLightDirection", glm::vec4(0.2, -1, 0.5, 1));
		mProgram.setUniform("uLightAmbient", glm::vec4(0.1, 0.1, 0.1, 1.0));
		mProgram.setUniform("uLightDiffuse", glm::vec4(0.85, 0.85, 0.85, 1.0));
		
		mTexture.bind(0);
		mProgram.setUniform("uTexture", 0);
		
		// draw the cube at 100 different locations
		for (int i = 0; i < 10; ++i)
			for (int j = 0; j < 10; ++j) {
				mProgram.setUniform("uModel", glm::translate(glm::vec3(2 * (i - 5), 0, 2 * (j - 5))));

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