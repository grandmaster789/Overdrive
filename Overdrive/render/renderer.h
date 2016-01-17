#pragma once

#include "texture2D.h"

namespace overdrive {
	namespace render {
		/*
			
		*/
		class Renderer {
		public:
			Renderer();
			virtual ~Renderer();

			void submit();
			
			void present();

		private:
			
		};
	}
}
