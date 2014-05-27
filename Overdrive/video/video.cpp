#include "video/video.h"
#include "core/engine.h"
#include "opengl.h"

namespace overdrive {
	namespace video {
		Video::Video() :
			System("Video")
		{
		}

		bool Video::initialize() {
			System::initialize();
			
			mEngine->updateSystem(this, true, false);

			return true;
		}

		void Video::update() {
			
		}

		void Video::shutdown() {
			System::shutdown();
		}
	}
}