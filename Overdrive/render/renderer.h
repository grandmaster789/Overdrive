#pragma once

#include "texture2D.h"
#include "shaderprogram.h"

namespace overdrive {
	namespace render {
		/*
			~ Shader management
			~ Texture management?
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
