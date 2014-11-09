#include "app/application.h"
#include "app/spinningcube.h"
#include "app/rendertest.h"
//#include "app/cameratest.h"

#include "core/engine.h"
#include "core/logger.h"

#include "video/video.h"
#include "input/input.h"

#include "test/test.h"

using namespace overdrive;

int main() {
	//overdrive::test::runAllTests();

	overdrive::core::Engine engine;
	
	engine.add(new overdrive::video::Video);
	engine.add(new overdrive::input::Input);

	engine.initialize(); // initialize all engine components so the application has a working environment

	//engine.setApplication(new overdrive::app::SpinningCube);
	engine.setApplication(new overdrive::app::RenderTest);
	//engine.setApplication(new overdrive::app::CameraTest);

	engine.run();
}
