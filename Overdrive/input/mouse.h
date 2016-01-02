#pragma once

#include <ostream>
#include <memory>
#include <utility>
#include "../opengl.h"

namespace overdrive {
	namespace video {
		class Window;
	}

	namespace input {
		/*
			[TODO] custom cursor shapes
			[TODO] higer-level events ~ dragging, double-click
		*/

		class Mouse {
		private:
			friend class video::Window;

			Mouse(video::Window* sourceWindow = nullptr);

		public:
			Mouse(const Mouse&) = delete;
			Mouse(Mouse&& m);
			Mouse& operator = (const Mouse&) = delete;
			Mouse& operator = (Mouse&& m);

			enum class eCursorState {
				VISIBLE,	// makes the cursor visible and behaving normally
				HIDDEN,		// makes the cursor invisible when it is over the client area of the window but does not restrict the cursor from leaving
				DISABLED	// hides and grabs the cursor, providing virtual and unlimited cursor movement
			};

			enum class eCursorShape {
				ARROW,
				IBEAM,
				CROSSHAIR,
				HAND,
				HRESIZE,
				VRESIZE
			};

			enum eButton: int {
				LEFT,
				RIGHT,
				MIDDLE
			};

			void setCursorState(eCursorState state);
			eCursorState getCursorState() const;
			void setStandardCursorShape(eCursorShape shape);
			void resetCursorShape();

			void setPosition(double x, double y); // in screencoordinates
			std::pair<double, double> getPosition() const; // [NOTE] clipped by the window bounds; or unbounded when in disabled input state

			// ----- Events -----
			struct OnEntered { Mouse* mMouse; video::Window* mWindow; };
			struct OnLeft { Mouse* mMouse; video::Window* mWindow; };
			
			struct OnButtonPress {
				Mouse* mMouse; 
				int mButton;
				int mModifiers;
			};

			struct OnButtonRelease {
				Mouse* mMouse;
				int mButton;
				int mModifiers;
			};

			struct OnMoved {
				Mouse* mMouse;
				double mPositionX;
				double mPositionY;
				double mDeltaX;
				double mDeltaY;
			};

			struct OnScroll {
				Mouse* mMouse;
				double mOffsetX;
				double mOffsetY;
			};

			double mPositionX;
			double mPositionY;
			bool mButtonState[GLFW_MOUSE_BUTTON_LAST];

		private:
			video::Window* mSourceWindow;
			std::unique_ptr<GLFWcursor, void(*)(GLFWcursor*)> mMouseCursor;

			
		};

		std::ostream& operator << (std::ostream& os, const Mouse::eCursorState& state);
		std::ostream& operator << (std::ostream& os, const Mouse::eCursorShape& shape);
	}
}