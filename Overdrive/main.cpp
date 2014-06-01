#include "core/engine.h"
#include "core/logger.h"

#include "video/video.h"
#include "input/input.h"
#include "input/keyboard.h"
#include "input/mouse.h"

#include "test/test.h"

struct KeyRecv :
	public overdrive::core::MessageHandler<overdrive::input::Keyboard::OnKeyPress>,
	public overdrive::core::MessageHandler<overdrive::input::Keyboard::OnKeyRelease>,
	public overdrive::core::MessageHandler<overdrive::input::Mouse::OnButtonPress>,
	public overdrive::core::MessageHandler<overdrive::input::Mouse::OnMove>,
	public overdrive::core::MessageHandler<overdrive::input::Mouse::OnEnter>,
	public overdrive::core::MessageHandler<overdrive::input::Mouse::OnLeave>,
	public overdrive::core::MessageHandler<overdrive::input::Mouse::OnScroll>
{
	void operator()(const overdrive::input::Keyboard::OnKeyPress& kp) {
		gLog << "received key: " << kp.mKey;

		if (kp.mKey == GLFW_KEY_ESCAPE)
			overdrive::core::Channel::broadcast(overdrive::core::Engine::OnStop());
	}
	
	void operator()(const overdrive::input::Keyboard::OnKeyRelease& kr) {
		gLog << "key released: " << kr.mKey;
	}

	void operator()(const overdrive::input::Mouse::OnButtonPress& mbp) {
		gLog << "mouse press: " << mbp.mButton;
	}

	void operator()(const overdrive::input::Mouse::OnMove& mm) {
		gLog << "move: " << 
			" (" << mm.mX << ", " << mm.mY << ") "
			" (" << mm.mDeltaX << ", " << mm.mDeltaY << ")";
	}

	void operator()(const overdrive::input::Mouse::OnEnter&) {
		gLog << "mouse entered";
	}

	void operator()(const overdrive::input::Mouse::OnLeave& ) {
		gLog << "mouse left";
	}

	void operator()(const overdrive::input::Mouse::OnScroll& scr) {
		gLog << "mouse scroll " << scr.mXOffset << ", " << scr.mYOffset;
	}
};

int main() {
	overdrive::core::Engine engine;
	
	KeyRecv recv;

	engine.add(new overdrive::input::Input);
	engine.add(new overdrive::video::Video);
	
	engine.run();
}