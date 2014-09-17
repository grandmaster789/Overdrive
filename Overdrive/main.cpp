#include "app/application.h"
#include "app/spinningcube.h"
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

int main() {
	//overdrive::test::runAllTests();

	overdrive::core::Engine engine;
	
	engine.add(new overdrive::video::Video);
	engine.add(new overdrive::input::Input);

	engine.setApplication(new overdrive::app::SpinningCube);
	//engine.setApplication(new overdrive::app::CameraTest);

	engine.run();
}
