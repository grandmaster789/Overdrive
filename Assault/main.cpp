#include "overdrive.h"
#include "render/renderstate.h"
#include "render/shaderprogram.h"
#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"
#include "render/vertexarray.h"
#include "video/video.h"
#include "scene/camera.h"

#include <iostream>

using namespace overdrive;

class Test :
	public app::Application,
	public MessageHandler<input::Keyboard::OnKeyPress>,
	public MessageHandler<input::Mouse::OnButtonPress>,
	public MessageHandler<input::Mouse::OnMoved>,
	public MessageHandler<input::Gamepad::OnButtonPressed>,
	public MessageHandler<input::Gamepad::OnButtonReleased>,
	public MessageHandler<input::Gamepad::OnLeftStickMoved>
{
public:
	int counter = 0;
	render::RenderState mRenderState;
	render::ShaderProgram mProgram;
	scene::Camera mCamera;

	std::unique_ptr<render::VertexBuffer<render::attributes::PositionColor>> mVBO;
	std::unique_ptr<render::IndexBuffer<GLuint>> mIBO;
	std::unique_ptr<render::VertexArray> mVAO;

	Test():
		Application("Test")
	{
		addDependency("Video");
	}

	virtual void initialize() override {
		System::initialize();
		
		mRenderState.setClearColor(0.2f, 0.2f, 0.0f);

		const char* vertex_shader =
			"#version 400\n"
			"in vec3 vertexPosition;\n"
			"in vec4 vertexColor;\n"
			"uniform mat4 viewMatrix;\n"
			"uniform mat4 projectionMatrix;\n"
			"out vec4 color;\n"
			"void main () {\n"
			"  gl_Position = projectionMatrix * viewMatrix * vec4(vertexPosition, 1.0);\n"
			"  color = vertexColor;\n"
			"}\n";

		const char* fragment_shader =
			"#version 400\n"
			"in vec4 color;\n"
			"out vec4 frag_color;\n"
			"void main () {\n"
			"   frag_color = color;\n"
			"}\n";

		mProgram.attachShader(vertex_shader, render::eShaderType::VERTEX);
		mProgram.attachShader(fragment_shader, render::eShaderType::FRAGMENT);

		mProgram.link();
		mProgram.bind();

		using render::attributes::PositionColor;

		mVBO = std::make_unique<render::VertexBuffer<PositionColor>>(3);
		{
			auto data = mVBO->map();
			data[0] = PositionColor{ glm::vec3(0.f, 0.5f, 0.0f),	glm::vec4(1, 0, 0, 1) };
			data[1] = PositionColor{ glm::vec3(0.5f, -0.5f, 0.0f),	glm::vec4(0, 1, 0, 1) };
			data[2] = PositionColor{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0, 0, 1, 1) };
		}

		mIBO = std::make_unique<render::IndexBuffer<GLuint>>(3);
		{
			auto data = mIBO->map();
			data[0] = 0;
			data[1] = 1;
			data[2] = 2;
		}

		mVAO = std::make_unique<render::VertexArray>();
		mVAO->attach(*mVBO);
		mVAO->attach(*mIBO);

		mCamera.setPosition(0.0f, 0.0f, 1.0f);
		mCamera.setViewportWindow(mEngine->get<video::Video>()->getMainWindow());
	}

	virtual void update() override {
		//gLog << mEngine->getClock();
		//std::cout << ".";

		//if (counter++ > 100)
			//mChannel.broadcast(overdrive::core::Engine::OnStop());
		glm::quat slightRotationZ = glm::quat_cast(glm::rotate(0.01f, glm::vec3(0, 0, 1)));
		mCamera.setPosition(mCamera.getPosition() + glm::vec3(0, 0, 0.01f));
		mCamera.setOrientation(slightRotationZ * mCamera.getOrientation());
		mCamera.update();

		mRenderState.clear();
		mProgram.setUniform("viewMatrix", mCamera.getView());
		mProgram.setUniform("projectionMatrix", mCamera.getProjection());
		mVAO->draw();
	}

	virtual void shutdown() override {
		System::shutdown();
	}

	void operator()(const input::Keyboard::OnKeyPress& kp) {
		gLog << "Keypress: " << kp.mKey;

		if (kp.mKey == GLFW_KEY_ESCAPE)
			mChannel.broadcast(core::Engine::OnStop());

		if (kp.mKey == GLFW_KEY_F1)
			gLog << mProgram;
	}

	void operator()(const input::Mouse::OnButtonPress& bp) {
		auto pos = bp.mMouse->getPosition();

		gLog << "Mouse press: (" << pos.first << ", " << pos.second << ") -> " << bp.mButton;
	}

	void operator()(const input::Mouse::OnMoved& mm) {
		gLog << "(" << mm.mPositionX << ", " << mm.mPositionY << ") - d(" << mm.mDeltaX << ", " << mm.mDeltaY << ")";
	}

	void operator()(const input::Gamepad::OnButtonPressed& bp) {
		using overdrive::input::Gamepad;

		gLog << "ButtonState: " << bp.mButtonState;

		if (bp.mButtonState & Gamepad::A)
			bp.mGamepad->setVibration(1.0f, 1.0f); // maximum vibration on
	}

	void operator()(const input::Gamepad::OnButtonReleased& br) {
		using overdrive::input::Gamepad;
		
		if (br.mButtonState & Gamepad::A)
			br.mGamepad->setVibration(0.0f, 0.0f); // switch vibration off
	}

	void operator()(const input::Gamepad::OnLeftStickMoved& stick) {
		gLog << "Left stick: (" << stick.mNewPosition.first << ", " << stick.mNewPosition.second << ")";
	}
};

int main() {
	gLog << "Starting Overdrive Assault";

	core::Engine engine;
	engine.add(new overdrive::video::Video);
	engine.add(new overdrive::input::Input);
	engine.setApplication(new Test);

	engine.run();

	gLog << "Done";
}