#pragma once

#include <string>
#include <memory>
#include "monitor.h"
#include "windowHints.h"
#include "../opengl.h"

namespace overdrive {
	namespace input {
		class Mouse;
		class Keyboard;
	}

	namespace video {
		/*
			[TODO] ostream
			[TODO] context sharing (for threaded loading of resources perhaps)
			[TODO] drag n drop events?
			[TODO] clipboard? copypasta?
		 */

		// http://www.glfw.org/docs/latest/window.html
		// http://www.glfw.org/docs/latest/group__window.html
		class Window {
		public:
			struct Frame {
				int mLeft;
				int mTop;
				int mRight;
				int mBottom;
			};

			struct ContextAttributes {
				eClientAPI mAPI;
				int mVersionMajor;
				int mVersionMinor;
				int mVersionRevision;
				bool mOpenGLForwardCompatible;
				bool mOpenGLDebugContext;
				eOpenGLProfile mProfile;
				eContextRobustness mContextRobustness;
				// [NOTE] apparantly the context release behavior is not part of this O_o
			};

			struct FramebufferAttributes {
				int mRedBits;
				int mGreenBits;
				int mBlueBits;
				int mAlphaBits;
				int mDepthBits;
				int mStencilBits;
				int mSamples; // MSAA
			};

			Window(const std::string& title, int width, int height);					// windowed mode (on primary monitor)
			Window(const std::string& title, const Monitor* m);							// borderless fullscreen mode (make sure that the decoration hint is turned off)
			Window(const std::string& title, const Monitor* m, int width, int height);	// fullscreen mode
			Window(const Window&) = delete;
			Window(Window&& w);
			Window& operator = (const Window&) = delete;
			Window& operator = (Window&& w);
			~Window();

			GLFWwindow* getHandle() const;

			bool shouldClose() const;

			void setShouldClose(bool value);
			void setTitle(const std::string& title);
			void setPosition(int x, int y); // client area position in screen coords
			void setSize(int width, int height); // client area size from left upper corner

			void iconify();
			void restore();
			void show();
			void hide();

			void makeCurrent();
			void swapBuffers();
						
			std::pair<int, int> getPosition() const; // in screen coords
			std::pair<int, int> getSize() const; // client area size in screen coords
			std::pair<int, int> getFramebufferSize() const; // in actual pixels; use this for viewport sizes
			int getFramebufferWidth() const;
			int getFramebufferHeight() const;
			Frame getFrame() const; // includes decorations, if any

			Monitor* getMonitor() const;
			input::Keyboard* getKeyboard() const;
			input::Mouse* getMouse() const;

			// ----- Attribute queries -----
			bool isFocused() const;
			bool isIconified() const;
			bool isVisible() const;
			bool isResizable() const; // resizable by the user that is
			bool isDecorated() const;
			bool isFloating() const; // also known as always-on-top

			ContextAttributes getContextAttributes() const;
			FramebufferAttributes getFramebufferAttributes() const;

			// ----- Events ------
			struct OnMoved {
				Window* mWindow; 
				int mOldPositionX;
				int mOldPositionY;
				int mPositionX;
				int mPositionY;
			};

			struct OnResized {
				Window* mWindow;
				int mOldWidth;
				int mOldHeight;
				int mWidth;
				int mHeight;
			};

			struct OnFramebufferResized {
				Window* mWindow; 
				int mWidth;
				int mHeight;
			};

			struct OnCreated { Window* mWindow; };
			struct OnRefreshed { Window* mWindow; };
			struct OnClosed { Window* mWindow; };
			struct OnFocused { Window* mWindow; };
			struct OnFocusLost { Window* mWindow; };
			struct OnIconify { Window* mWindow; };
			struct OnRestore { Window* mWindow; };

			// ----- Handlers ------
			void operator()(const OnMoved& moved);
			void operator()(const OnResized& resized);

		private:
			GLFWwindow* mHandle;

			std::string mTitle;

			int mPositionX;
			int mPositionY;
			int mWidth;
			int mHeight;

			std::unique_ptr<input::Keyboard> mKeyboard;
			std::unique_ptr<input::Mouse> mMouse;
		};
	}
}
