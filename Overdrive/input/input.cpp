#include "input/input.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "core/engine.h"
#include "video/video.h"
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

			auto vid = mEngine->get("Video");
			
			if (vid) {
				auto vidSystem = static_cast<overdrive::video::Video*>(vid);

				for (auto& w : vidSystem->getWindows()) {
					addKeyboard(Keyboard(&w));
					addMouse(Mouse(&w));
				}
			}

			return true;
		}

		void Input::update() {
			glfwPollEvents();
		}

		void Input::shutdown() {
			System::shutdown();
		}

		void Input::addKeyboard(Keyboard&& kb) {
			mKeyboards.emplace_back(std::move(kb));
		}

		void Input::addMouse(Mouse&& m) {
			mMice.emplace_back(std::move(m));
		}

		void Input::operator()(const video::Window::OnCreate& onCreate) {
			addKeyboard(Keyboard(onCreate.window));
			addMouse(Mouse(onCreate.window));
		}

		void Input::operator()(const video::Window::OnClose& onClose) {
			unregisterKeyboard(onClose.window);
			unregisterMouse(onClose.window);

			std::remove_if(
				mKeyboards.begin(), 
				mKeyboards.end(), 
				[onClose](const Keyboard& kb) { 
					return kb.isAssociatedWith(onClose.window); 
				}
			);

			std::remove_if(
				mMice.begin(),
				mMice.end(),
				[onClose](const Mouse& m) {
					return m.isAssociatedWith(onClose.window);
				}
			);
		}
	}
}