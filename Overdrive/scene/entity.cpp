#include "stdafx.h"
#include "entity.h"

namespace overdrive {
	namespace scene {
		namespace {
			static EntityID gNextEntityID = 0;
		}

		Entity::Entity():
			mID(gNextEntityID++)
		{
		}

		EntityID Entity::getID() const {
			return mID;
		}
	}
}