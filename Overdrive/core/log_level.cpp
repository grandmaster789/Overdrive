#include "log_level.h"
#include <atomic>

namespace overdrive {
	namespace core {
		namespace {
			std::atomic<bool> gLogLevelState[5]{
				{ true }, // DEBUG
				{ true }, // MESSAGE
				{ true }, // WARNING
				{ true }, // ERROR_
				{ true }  // FATAL
			};

			int selectLogState(eLogLevel level) {
				switch (level) {
				case eLogLevel::DEBUG:		return 0;
				case eLogLevel::MESSAGE:	return 1;
				case eLogLevel::WARNING:	return 2;
				case eLogLevel::ERROR_:		return 3;
				case eLogLevel::FATAL:		return 4;
				default:
					throw std::runtime_error("Unsupported log level");
				}
			}
		}

		void setLogLevel(eLogLevel level, bool enabled) {
			int lvl = selectLogState(level);
			gLogLevelState[lvl].store(enabled, std::memory_order_release);
		}

		bool logLevel(eLogLevel level) {
			int lvl = selectLogState(level);
			return gLogLevelState[lvl].load(std::memory_order_acquire);
		}
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::core::eLogLevel& level) {
	using overdrive::core::eLogLevel;

	// Chosen so you'll have an easy time spotting them in the actual log

	switch (level) {
	case eLogLevel::DEBUG:		os << "[dbg] ";		break;
	case eLogLevel::MESSAGE:	os << "      ";		break; // this should be the majority during debugging. Should be very non-conspicous
	case eLogLevel::WARNING:	os << "*wrn* ";		break;
	case eLogLevel::ERROR_:		os << "< ERROR >      ";	break;	// extra noticable
	case eLogLevel::FATAL:		os << "<## FATAL ##>  ";	break;	// extra noticable
	default:
		os << "Unknown";
	}

	return os;
}