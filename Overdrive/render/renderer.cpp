#include "stdafx.h"
#include "renderer.h"
#include <boost/math/constants/constants.hpp>

namespace overdrive {
	namespace render {
		Renderer::Renderer(video::Window* associatedWindow):
			mWindow(associatedWindow)
		{
	
		}

		void Renderer::submit() {
			
		}

		void Renderer::present() {
			mState.clear(eClearOptions::DEPTH);
			
			mWindow->swapBuffers();
		}
	}
}