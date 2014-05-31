#include "core/engine.h"

#include "video/video.h"
#include "input/input.h"

#include "test/test.h"

int main() {
	overdrive::core::Engine engine;

	engine.add(new overdrive::video::Video);
	engine.add(new overdrive::input::Input);
	engine.run();
}