#include "overdrive.h"
#include "render/renderstate.h"
#include "render/shaderprogram.h"

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

	GLuint vbo = 0;
	GLuint vao = 0;

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
			"void main () {\n"
			"  gl_Position = vec4 (vp, 1.0);\n"
			"}\n";

		const char* fragment_shader =
			"#version 400\n"
			"out vec4 frag_colour;\n"
			"void main () {\n"
			"  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);\n"
			"}\n";

		mProgram.attachShader(vertex_shader, render::eShaderType::VERTEX);
		mProgram.attachShader(fragment_shader, render::eShaderType::FRAGMENT);

		mProgram.link();
		mProgram.bind();
		
		float points[] = {
			0.0f,  0.5f,  0.0f,
			0.5f, -0.5f,  0.0f,
			-0.5f, -0.5f,  0.0f
		};

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	virtual void update() override {
		//gLog << mEngine->getClock();
		//std::cout << ".";

		//if (counter++ > 100)
			//mChannel.broadcast(overdrive::core::Engine::OnStop());

		mRenderState.clear();

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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