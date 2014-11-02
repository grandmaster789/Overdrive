#ifndef OVERDRIVE_RENDER_RENDER_H
#define OVERDRIVE_RENDER_RENDER_H

#include "core/system.h"

namespace overdrive {
	namespace render {
		class Render :
			public core::System
		{
		public:
			Render();

			virtual bool initialize() override;
			virtual void update() override;
			virtual void shutdown() override;
		};
	}
}

#endif
