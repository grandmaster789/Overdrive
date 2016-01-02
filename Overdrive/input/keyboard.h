#pragma once

#include "../opengl.h"
#include <array>

namespace overdrive {
	namespace video {
		class Window;
	}

	namespace input {
		/*
			A proxy for the keyboard state, given that a Window has focus
			[NOTE] the public keystate is somewhat unfortunate, but the key callback requires it and I don't feel like tangling with the priveleges right now
			[TODO] unicode input?
			[TODO] move semantics? does that even make sense?
		*/
		class Keyboard {
		private:
			friend class video::Window;

			Keyboard(video::Window* sourceWindow = nullptr);
			
		public:
			Keyboard(const Keyboard&) = delete;
			Keyboard& operator = (const Keyboard&) = delete;

			void setStickyKeys(bool enabled);
			bool getStickyKeys() const;

			video::Window* getSourceWindow() const;

			// For an overview of keycodes, see http://www.glfw.org/docs/latest/group__keys.html
			// An overview of modifiers can be found at http://www.glfw.org/docs/latest/group__mods.html

			bool operator[](int button) const; // ex: keyboard[GLFW_BUTTON_ESCAPE] == false

			// ----- Events -----
			struct OnKeyPress {
				Keyboard* mKeyboard;
				int mKey;
				int mModifiers;
			};

			struct OnKeyRelease {
				Keyboard* mKeyboard;
				int mKey;
				int mModifiers;
			};

			bool mKeyState[GLFW_KEY_LAST];

		private:
			video::Window* mSourceWindow;
		};
	}
}
