#ifndef OVERDRIVE_VIDEO_WINDOW_H
#define OVERDRIVE_VIDEO_WINDOW_H

#include <string>
#include <unordered_map>

#include "opengl.h"

#include "video/monitor.h"

namespace overdrive {
	namespace video {
		class Window {
		public:
			enum class eClientAPI {
				OPENGL_API,
				OPENGL_ES_API
			};

			enum class eContextRobustness {
				NO_ROBUSTNESS,
				NO_RESET_NOTIFICATION,
				LOSE_CONTEXT_ON_RESET
			};

			enum class eOpenGLProfile {
				OPENGL_ANY_PROFILE,
				OPENGL_COMPATIBILITY_PROFILE,
				OPENGL_CORE_PROFILE
			};

			Window();

			void setWidth(int newWidth);
			void setHeight(int newHeight);
			int getWidth() const;
			int getHeight() const;

			bool isFullscreen() const; // setFullscreen may introduce problems, so I'll leave it as a todo

			const std::string& getTitle() const;

			void create();
			void createFullscreen(Monitor* m);
			void destroy();
			
			void swapBuffers(); //note that this both swaps buffers and handles basic input
			bool shouldClose() const;

			// Signals
			struct OnCreate	{ Window* window; };
			struct OnFramebufferResize { Window* window; int newWidth, newHeight; };
			struct OnClose { Window* window; };
			struct OnFocus { Window* window; };
			struct OnDeFocus { Window* window; };
			struct OnIconify { Window* window; };
			struct OnRestore { Window* window; };
			struct OnMove { Window* window; int newX, newY; };
			struct OnRefresh { Window* window; };
			struct OnResize { Window* window; int newWidth, newHeight; };

			// Window Attributes
			bool isFocused() const;
			bool isIconified() const;
			bool isVisible() const;
			bool isResizable() const;
			bool isDecorated() const;
			
			std::string getWindowTitle() const;
			void getWindowPosition(int& x, int& y) const;
			void getWindowSize(int& width, int& height) const;
			void getFramebufferSize(int& width, int& height);

			// Context Attributes
			eClientAPI getClientAPI() const;
			int getContextVersionMajor() const;
			int getContextVersionMinor() const;
			int getContextRevision() const;
			bool isOpenGLForwardCompatible() const;
			bool isOpenGLDebugContext() const;
			eContextRobustness getContextRobustness() const;
			eOpenGLProfile getOpenGLProfile() const;

			// Triggers
			void setTitle(std::string title);
			void iconify();
			void restore();
			void show();
			void hide();

			struct CreationHints {
				void apply(); // checks and applies the current settings

				bool mResizable = true;
				bool mVisible = true;
				bool mDecorated = true;

				int mRedBits = 8;
				int mGreenBits = 8;
				int mBlueBits = 8;
				int mAlphaBits = 8;
				int mDepthBits = 24;
				int mStencilBits = 8;

				int mAccumulationRedBits = 0;
				int mAccumulationGreenBits = 0;
				int mAccumulationBlueBits = 0;
				int mAccumulationAlphaBits = 0;

				int mAuxiliaryBuffers = 0;

				int mSamples = 0;
				int mRefreshRate = 0;

				bool mStereo = false;

				bool mSRGB = false;

				eClientAPI mClientAPI = eClientAPI::OPENGL_API;

				int mContextVersionMajor = 1;	
				int mContextVersionMinor = 0;
				// [Note] - When requesting an openGL profile below 3.2, you *must* also use the OPENGL_ANY_PROFILE

				eContextRobustness mContextRobustness = eContextRobustness::NO_ROBUSTNESS;

				bool mOpenGLForwardCompatible = false;
				bool mOpenGLDebugContext = false;

				eOpenGLProfile mOpenGLProfile = eOpenGLProfile::OPENGL_ANY_PROFILE;
			};

			void setDefaultCreationHints();
			static CreationHints mCreationHints; // this should be done better I guess

			static Window* getFromHandle(GLFWwindow* handle);

		private:
			GLFWwindow* mHandle;

			static std::unordered_map<GLFWwindow*, Window*> mHandleRegistry;

			std::string mTitle = "Default Window";

			int mWidth = 800;
			int mHeight = 600;
			bool mIsFullscreen = false;

			Monitor* mMonitor = nullptr; 
			Window* mSharedContext = nullptr;
		};
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::video::Window::eClientAPI& api);
std::ostream& operator << (std::ostream& os, const overdrive::video::Window::eContextRobustness& robust);
std::ostream& operator << (std::ostream& os, const overdrive::video::Window::eOpenGLProfile& profile);

#endif
