#include "named.h"

namespace overdrive {
	namespace util {
		Named::Named(std::string name) :
			mName{ std::move(name) }
		{
		}

		const std::string& Named::getName() const {
			return mName;
		}

		void Named::setName(std::string name) {
			mName = std::move(name);
		}
	}
}