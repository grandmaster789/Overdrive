#include "overdrive.h"
#include "render/renderstate.h"
#include "render/shaderprogram.h"
#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"
#include "render/vertexarray.h"

#include <iostream>

using namespace overdrive;

class Test :
	public app::Application,
	public MessageHandler<video::Window::OnResized>,
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

	std::unique_ptr<render::VertexBuffer<render::attributes::PositionColor>> mVBO;
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
			"in vec3 vp;\n"
			"in vec4 vc;\n"
			"out vec4 color;\n"
			"void main () {\n"
			"  gl_Position = vec4 (vp, 1.0);\n"
			"  color = vc;\n"
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

		mVAO = std::make_unique<render::VertexArray>();
		mVAO->attach(*mVBO);
	}

	virtual void update() override {
		//gLog << mEngine->getClock();
		//std::cout << ".";

		//if (counter++ > 100)
			//mChannel.broadcast(overdrive::core::Engine::OnStop());

		mRenderState.clear();
		mVAO->draw();
	}

	virtual void shutdown() override {
		System::shutdown();
	}

	void operator()(const video::Window::OnResized& msg) {
		gLog << "Window resized: " << msg.mWidth << "x" << msg.mHeight;
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