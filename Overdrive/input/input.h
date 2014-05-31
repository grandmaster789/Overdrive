#ifndef OVERDRIVE_INPUT_INPUT_H
#define OVERDRIVE_INPUT_INPUT_H

#include "core/system.h"

namespace overdrive {
	namespace input {
		class Input:
			public core::System
		{
		public:
			Input();

			virtual bool initialize() override;
			virtual void update() override;
			virtual void shutdown() override;
		};
	}
}

#endif
