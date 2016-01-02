#pragma once

#include <ostream>

namespace overdrive {
	namespace core {
		enum class eLogLevel {
			DEBUG,
			MESSAGE,
			WARNING,
			ERROR_, // the underscore is because of the existance of a macro called ERROR...
			FATAL
		};

		void setLogLevel(eLogLevel level, bool enabled);
		bool logLevel(eLogLevel level);
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::core::eLogLevel& level);
