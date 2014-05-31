#include "input/input.h"
#include "core/engine.h"
#include "opengl.h"

namespace overdrive {
	namespace input {
		Input::Input():
			System("Input")
		{
		}

		bool Input::initialize() {
			System::initialize();

			mEngine->updateSystem(this, true, false); 

			return true;
		}

		void Input::update() {
			glfwPollEvents();
		}

		void Input::shutdown() {
			System::shutdown();
		}
	}
}