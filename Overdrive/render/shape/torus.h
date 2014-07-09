#ifndef OVERDRIVE_RENDER_SHAPE_TORUS_H
#define OVERDRIVE_RENDER_SHAPE_TORUS_H

#include "render/drawable.h"

namespace overdrive {
	namespace render {
		namespace shape {
			class Torus :
				public Drawable
			{
			public:
				Torus(
					float outerRadius = 0.5f,
					float innerRadius = 0.25f,
					size_t numRings = 20,
					size_t numSides = 20
				);

				virtual void draw() const override;

			private:
				size_t mNumFaces;
				size_t mNumRings;
				size_t mNumSides;
			};
		}
	}
}

#endif
