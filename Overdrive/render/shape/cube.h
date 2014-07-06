#ifndef OVERDRIVE_RENDER_SHAPE_CUBE_H
#define OVERDRIVE_RENDER_SHAPE_CUBE_H

#include "opengl.h"
#include "render/drawable.h"

namespace overdrive {
	namespace render {
		namespace shape {
			class Cube: public Drawable {
			public:
				Cube(float size = 1.0f);

				virtual void draw() const override;

			private:
				size_t mNumElements;
			};
		}
	}
}

#endif
