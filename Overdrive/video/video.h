#ifndef OVERDRIVE_VIDEO_VIDEO_H
#define OVERDRIVE_VIDEO_VIDEO_H

#include "core/system.h"

#include "video/window.h"

namespace overdrive {
	namespace video {
		/*
			The Video subsystem is basically an object-oriented wrapper around GLFW window handling
		 */

		class Video:
			public core::System
		{
		public:
			Video();

			virtual bool initialize() override;
			virtual void update() override;
			virtual void shutdown() override;
		};
	}
}

#endif
