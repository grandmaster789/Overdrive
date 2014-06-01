#ifndef OVERDRIVE_INPUT_INPUT_H
#define OVERDRIVE_INPUT_INPUT_H

#include "core/system.h"
#include "video/window.h"
#include <memory>
#include <vector>

namespace overdrive {
	namespace input {
		class Keyboard;

		class Input:
			public core::System,
			public core::MessageHandler<video::Window::OnCreate>,
			public core::MessageHandler<video::Window::OnClose>
		{
		public:
			Input();

			virtual bool initialize() override;
			virtual void update() override;
			virtual void shutdown() override;

			void addKeyboard(Keyboard&& kb); //this takes ownership of the keyboard

			void operator()(const video::Window::OnCreate& onCreate);
			void operator()(const video::Window::OnClose& onClose);

		private:
			std::vector<Keyboard> mKeyboards; //one keyboard object per window is maintaned
		};
	}
}

#endif
