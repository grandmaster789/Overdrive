#include "core/engine.h"
#include "core/logger.h"

#include "video/video.h"
#include "input/input.h"
#include "input/keyboard.h"
#include "input/mouse.h"

#include "test/test.h"

struct KeyRecv :
	public overdrive::core::MessageHandler<overdrive::input::Keyboard::OnKeyPress>,
	public overdrive::core::MessageHandler<overdrive::input::Mouse::OnButtonPress>,
	public overdrive::core::MessageHandler<overdrive::input::Mouse::OnMove>
{
	void operator()(const overdrive::input::Keyboard::OnKeyPress& kp) {
		gLog << "received key: " << kp.mKey;

		if (kp.mKey == GLFW_KEY_ESCAPE)
			overdrive::core::Channel::broadcast(overdrive::core::Engine::OnStop());
	}

	void operator()(const overdrive::input::Mouse::OnButtonPress& mbp) {
		gLog << "mouse press: " << mbp.mButton;
	}

	void operator()(const overdrive::input::Mouse::OnMove& mm) {
		gLog << "move: " << 
			" (" << mm.mX << ", " << mm.mY << ") "
			" (" << mm.mDeltaX << ", " << mm.mDeltaY << ")";
	}
};

int main() {
	overdrive::core::Engine engine;
	
	KeyRecv recv;

	engine.add(new overdrive::input::Input);
	engine.add(new overdrive::video::Video);
	
	engine.run();
}