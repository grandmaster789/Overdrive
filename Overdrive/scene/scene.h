#pragma once

#include "../core/system.h"

namespace overdrive {
	class Scene:
		public core::System
	{
	public:
		Scene();
		
		virtual void initialize() override;
		virtual void update() override;
		virtual void shutdown() override;

	private:

	};
}
