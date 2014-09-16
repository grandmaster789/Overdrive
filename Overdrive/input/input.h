#ifndef OVERDRIVE_INPUT_INPUT_H
#define OVERDRIVE_INPUT_INPUT_H

#include "core/system.h"
#include "core/channel.h"

#include "video/window.h"

#include "input/joystick.h"

#include "opengl.h" // for GLFW_JOYSTICK_LAST

namespace overdrive {
	namespace input {
		class Keyboard;
		class Mouse;

		class Input :
			public core::System,
			public core::MessageHandler<video::Window::OnCreate>,
			public core::MessageHandler<video::Window::OnClose>
		{
		public:
			typedef std::vector<Keyboard*> KeyboardList;
			typedef std::vector<Mouse*> MouseList;

			Input();

			virtual bool initialize() override;
			virtual void update() override;
			virtual void shutdown() override;

			const KeyboardList& getKeyboardList() const;
			const MouseList& getMouseList() const;

			void operator()(const video::Window::OnCreate& created);
			void operator()(const video::Window::OnClose& closed);

		private:
			KeyboardList mKeyboardList;
			MouseList mMouseList;
			Joystick mJoystickList[GLFW_JOYSTICK_LAST];
		};
	}
}

#endif
