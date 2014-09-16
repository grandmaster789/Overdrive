#include "app/application.h"
//#include "app/spinningcube.h"
//#include "app/cameratest.h"

#include "core/engine.h"
#include "core/logger.h"

#include "video/video.h"
#include "input/input.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/joystick.h"

#include "test/test.h"

#include <sstream>

using namespace overdrive;

class TestApp :
	public app::Application,
	public core::MessageHandler<input::Keyboard::OnKeyPress>,
	public core::MessageHandler<input::Mouse::OnButtonPress>,
	public core::MessageHandler<input::Mouse::OnEnter>,
	public core::MessageHandler<input::Mouse::OnScroll>,
	public core::MessageHandler<input::Mouse::OnMove>,
	public core::MessageHandler<input::Joystick::OnConnect>,
	public core::MessageHandler<input::Joystick::OnButtonPress>,
	public core::MessageHandler<input::Joystick::OnMove>
{
public:
	TestApp() :
		Application("Test")
	{
	}

	bool initialize() {
		System::initialize();
		return true;
	}

	void update() {
	}

	void shutdown() {
		System::shutdown();
	}

	void operator()(const input::Keyboard::OnKeyPress& kp) {
		gLog << "Key pressed: " << kp.mKey;

		if (kp.mKey == GLFW_KEY_ESCAPE)
			mChannel.broadcast(core::Engine::OnStop {});
	}

	void operator()(const input::Mouse::OnButtonPress& mbp) {
		gLog << "Mouse button pressed: " << mbp.mButton;
	}

	void operator()(const input::Mouse::OnEnter& ) {
		gLog << "Mouse entered client area";
	}

	void operator()(const input::Mouse::OnScroll& scroll) {
		gLog << "scroll: " << scroll.mXOffset << " | " << scroll.mYOffset;
	}

	void operator()(const input::Mouse::OnMove& mm) {
		gLog << 
			"move: (" << mm.mX << ", " << mm.mY << ") /" << 
			"[" << mm.mDeltaX << ", " << mm.mDeltaY << "]";
	}

	void operator()(const input::Joystick::OnConnect& connected) {
		gLog << "Joystick connected: " << connected.mJoystick->getName();
	}

	void operator()(const input::Joystick::OnButtonPress& bp) {
		gLog << "Joystick button: " << bp.mButtonID;
	}

	void operator()(const input::Joystick::OnMove& jm) {
		std::stringstream sstr;

		for (const auto& value: jm.mPosition)
			sstr << value << ", ";

		gLog << "Joystick moved: " << sstr.str();
	}
};

int main() {
	//overdrive::test::runAllTests();

	overdrive::core::Engine engine;
	
	engine.add(new overdrive::video::Video);
	engine.add(new overdrive::input::Input);

	engine.setApplication(new TestApp);
	//engine.setApplication(new overdrive::app::SpinningCube);
	//engine.setApplication(new overdrive::app::CameraTest);

	engine.run();
}
