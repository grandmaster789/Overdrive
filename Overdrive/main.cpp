#include "core/engine.h"
#include "core/logger.h"

#include "video/video.h"
#include "input/input.h"
#include "input/keyboard.h"

#include "test/test.h"

struct KeyRecv :
	public overdrive::core::MessageHandler<overdrive::input::Keyboard::KeyPress>
{
	void operator()(const overdrive::input::Keyboard::KeyPress& kp) {
		gLog << "received key: " << kp.mKey;

		if (kp.mKey == GLFW_KEY_ESCAPE)
			overdrive::core::Channel::broadcast(overdrive::core::Engine::OnStop());
	}
};

int main() {
	overdrive::core::Engine engine;
	
	KeyRecv recv;

	engine.add(new overdrive::input::Input);
	engine.add(new overdrive::video::Video);
	
	engine.run();
}