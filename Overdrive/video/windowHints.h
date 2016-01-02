#pragma once

#include "../opengl.h"
#include <ostream>

namespace overdrive {
	namespace video {
		enum class eClientAPI: int {
			OPENGL,
			OPENGL_ES
		};

		enum class eContextRobustness: int {
			NO_ROBUSTNESS,
			NO_RESET_NOTIFICATION,
			LOSE_CONTEXT_ON_RESET
		};

		enum class eContextReleaseBehavior: int {
			ANY,		// the default behavior of the context creation API will be used
			FLUSH,		// the pipeline will be flushed whenever the context is released from being the current one
			NONE		// the pipeline will not be flushed on release
		};

		enum class eOpenGLProfile: int {
			ANY,
			COMPATIBILITY,
			CORE
		};

		class Monitor;

		class WindowHints {
		private:
			friend class Video;

			WindowHints();

		public:
			void resetToDefaults(); // to GLFW defaults (notably openGL 1.0!)
			void fromCurrentVideoMode(const Monitor* m); // acquires the current bitsettings and refresh rate for a given monitor (for borderless fullscreen windows)
			void apply(); // applies the current settings for the next window to be created

			friend std::ostream& operator << (std::ostream& os, const WindowHints& hints);

			// the following values reflect the defaults from GLFW

			// ----- Window related -----
			bool mResizable		= true; // specifies whether the windowed mode window will be resizable by the user. 
			bool mVisible		= true; // specifies whether the windowed mode window will be initially visible. This hint is ignored for full screen windows
			bool mDecorated		= true; // specifies whether the windowed mode window will have window decorations such as a border, a close widget, etc. An undecorated window may still allow the user to generate close events on some platforms. This hint is ignored for full screen windows
			bool mFocused		= true; // specifies whether the windowed mode window will be given input focus when created. This hint is ignored for full screen and initially hidden windows
			bool mAutoIconify	= true; // specifies whether the full screen window will automatically iconify and restore the previous video mode on input focus loss.This hint is ignored for windowed mode windows
			bool mFloating		= false;// specifies whether the windowed mode window will be floating above other regular windows, also called topmost or always-on-top. This is intended primarily for debugging purposes and cannot be used to implement proper full screen windows. This hint is ignored for full screen windows

			// ----- Framebuffer related -----
			// specify the desired bit depths of the various components of the default framebuffer; use -1 if there's no preference
			int mRedBits		= 8;
			int mGreenBits		= 8;
			int mBlueBits		= 8;
			int mAlphaBits		= 8;
			int mDepthBits		= 24;
			int mStencilBits	= 8;

			// Accumulation buffers are a legacy OpenGL feature and should not be used in new code
			int mAccumRedBits	= 0;
			int mAccumGreenBits = 0;
			int mAccumBlueBits	= 0;
			int mAccumAlphaBits = 0;

			// Auxiliary buffers are a legacy OpenGL feature and should not be used in new code
			int mAuxBuffers		= 0;
			
			bool mStereo		= false;	// specifies whether to use stereoscopic rendering.This is a hard constraint
			int mSamples		= 0;		// specifies the desired number of samples to use for multisampling. 0 disables multisampling. -1 means the application has no preference
			bool mSRGBCapable	= false;	// specifies whether the framebuffer should be sRGB capable
			bool mDoubleBuffer	= true;		// specifies whether the framebuffer should be double buffered.You nearly always want to use double buffering.This is a hard constraint

			// ----- Monitor related -----
			int mRefreshRate	= 0;		// specifies the desired refresh rate for full screen windows. If set to -1, the highest available refresh rate will be used. This hint is ignored for windowed mode windows

			// ----- Context related -----
			eClientAPI mClientAPI		= eClientAPI::OPENGL;	// specifies which client API to create the context for. This is a hard constraint
			int mContextVersionMajor	= 1;		// specify the client API version that the created context must be compatible with. The exact behavior of these hints depend on the requested client API
			int mContextVersionMinor	= 0;

			bool mOpenGLForwardCompatible = false;	// specifies whether the OpenGL context should be forward-compatible, i.e. one where all functionality deprecated in the requested version of OpenGL is removed. This may only be used if the requested OpenGL version is 3.0 or above. If OpenGL ES is requested, this hint is ignored
			bool mOpenGLDebugContext = false;		// specifies whether to create a debug OpenGL context, which may have additional error and performance issue reporting functionality. If OpenGL ES is requested, this hint is ignored
			eOpenGLProfile mOpenGLProfile = eOpenGLProfile::ANY; // specifies which OpenGL profile to create the context for. When requesting an openGL profile below v3.2 you must use ANY_PROFILE. If OpenGL ES is requested, this hint is ignored
			eContextRobustness mContextRobustness = eContextRobustness::NO_ROBUSTNESS; // specifies the robustness strategy to be used by the context
			eContextReleaseBehavior mContextReleaseBehavior = eContextReleaseBehavior::ANY; // specifies the release behavior to be used by the context
		};

		std::ostream& operator << (std::ostream& os, const eClientAPI& value);
		std::ostream& operator << (std::ostream& os, const eContextRobustness& value);
		std::ostream& operator << (std::ostream& os, const eContextReleaseBehavior& value);
		std::ostream& operator << (std::ostream& os, const eOpenGLProfile& value);
	}
}


