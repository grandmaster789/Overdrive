#pragma once

namespace overdrive {
	namespace scene {
		class Component {
		public:
			friend class Entity;

			virtual ~Component() {}
		};
	}
}
