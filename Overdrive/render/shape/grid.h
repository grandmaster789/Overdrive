#ifndef OVERDRIVE_RENDER_SHAPE_GRID_H
#define OVERDRIVE_RENDER_SHAPE_GRID_H

#include "render/drawable.h"

namespace overdrive {
	namespace render {
		namespace shape {
			class Grid: public Drawable {
			public:
				Grid(
					float xSize = 1.0f,
					float zSize = 1.0f,
					size_t numXDivs = 10,
					size_t numZDivs = 10
				);

				virtual void draw() const override;

			private:
				size_t mNumIndices;
			};
		}
	}
}

#endif
