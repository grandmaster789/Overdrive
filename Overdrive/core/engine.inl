#ifndef OVERDRIVE_CORE_ENGINE_INL
#define OVERDRIVE_CORE_ENGINE_INL

#include "core/engine.h"
#include "util/exception.h"

namespace overdrive {
	namespace core {
		// Note -- if systems are removed regularly, from non-main threads this needs to become threadsafe as well
		//		   I don't expect this to be the typical use case, so I'll leave it for later

		template <typename T>
		T* Engine::get() const {
			T* result = nullptr;

			for (const auto& system : mSystems) {
				result = dynamic_cast<T*>(system.get());

				if (result)
					return result;
			}

			throw std::exception("System type not found");
		}
	}
}

#endif
