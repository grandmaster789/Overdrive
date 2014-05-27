#ifndef OVERDRIVE_VIDEO_WINDOW_H
#define OVERDRIVE_VIDEO_WINDOW_H

#include <string>

#include "opengl.h"

#include "video/monitor.h"

namespace overdrive {
	namespace video {
		class Window {
		public:
			Window();

			int getWidth() const;
			int getHeight() const;
			bool isFullscreen() const;

			const std::string& getTitle() const;

			void create();
			void createFullscreen(Monitor* m);
			
			void swapBuffers();
			bool shouldClose() const;

			struct OnCreate	{ Window* window; };
			struct OnFramebufferResize { Window* window; int newWidth, newHeight; };
			struct OnClose { Window* window; };
			struct OnFocus { Window* window; };
			struct OnIconify { Window* window; };
			struct OnMove { Window* window; int newX, newY; };
			struct OnRefresh { Window* window; };
			struct OnResize { Window* window; int newWidth, newHeight; };

			struct CreationHints {
				void apply();

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

				enum class eClientAPI {
					OPENGL_API,
					OPENGL_ES_API
				} mClientAPI = eClientAPI::OPENGL_API;

				int mContextVersionMajor = 1;	
				int mContextVersionMinor = 0;
				// [Note] - When requesting an openGL profile below 3.2, you *must* also use the OPENGL_ANY_PROFILE

				enum class eContextRobustness {
					NO_ROBUSTNESS,
					NO_RESET_NOTIFICATION,
					LOSE_CONTEXT_ON_RESET
				} mContextRobustness = eContextRobustness::NO_ROBUSTNESS;

				bool mOpenGLForwardCompatible = false;
				bool mOpenGLDebugContext = false;

				enum class eOpenGLProfile {
					OPENGL_ANY_PROFILE,
					OPENGL_COMPATIBILITY_PROFILE,
					OPENGL_CORE_PROFILE
				} mOpenGLProfile = eOpenGLProfile::OPENGL_ANY_PROFILE;
			};

			static CreationHints mCreationHints; // this should be done better I guess

		private:
			GLFWwindow* mHandle;

			std::string mTitle = "Default Window";

			int mWidth = 800;
			int mHeight = 600;
			bool mIsFullscreen = false;

			Monitor* mMonitor = nullptr; 
			Window* mSharedContext = nullptr;
		};
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::video::Window::CreationHints::eClientAPI& api);
std::ostream& operator << (std::ostream& os, const overdrive::video::Window::CreationHints::eContextRobustness& robust);
std::ostream& operator << (std::ostream& os, const overdrive::video::Window::CreationHints::eOpenGLProfile& profile);

#endif
