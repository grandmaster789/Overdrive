#include "stdafx.h"
#include "shape_fullquad.h"

namespace overdrive {
	namespace render {
		namespace shape {
			FullQuad::FullQuad():
				mVBO({
					attributes::PositionTexCoord{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
					attributes::PositionTexCoord{ glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
					attributes::PositionTexCoord{ glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },

					attributes::PositionTexCoord{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
					attributes::PositionTexCoord{ glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
					attributes::PositionTexCoord{ glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f) }
				})
			{
				mVAO.attach(mVBO);
			}

			void FullQuad::draw() {
				mVAO.draw();
			}
		}
	}
}