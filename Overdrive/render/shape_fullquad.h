#pragma once

#include "vertexarray.h"

namespace overdrive {
	namespace render {
		namespace shape {
			/*
				[NOTE]	This shape is intended for use with (post-processing) effects - fullscreen/rtt and such. 
						As such it has a different vertex format than the rest of the shapes
			*/

			class FullQuad {
			public:
				FullQuad();

				void draw();

			private:
				VertexBuffer<attributes::PositionTexCoord> mVBO;
				VertexArray mVAO;
			};
		}
	}
}
