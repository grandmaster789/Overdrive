#pragma once

namespace overdrive {
	namespace scene {
		typedef unsigned long long EntityID;

		class Entity {
		public:
			Entity();

			EntityID getID() const;

		private:
			EntityID mID;
		};
	}
}
