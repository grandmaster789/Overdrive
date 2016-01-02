#pragma once

#include "engine.h"
#include <exception>

namespace overdrive {
	namespace core {
		// NOTE -- this is neither fast nor threadsafe, but may prove useful
		template <typename T>
		T* Engine::get() const {
			T* result = nullptr;

			for (const auto& system : mSystems) {
				result = dynamic_cast<T*>(system.get());

				if (result)
					return result;
			}

			throw std::runtime_error("System type not found");
		}
	}
}
