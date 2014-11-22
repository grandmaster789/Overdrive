#ifndef OVERDRIVE_CORE_LOGGER_INL
#define OVERDRIVE_CORE_LOGGER_INL

#include "core/logger.h"

namespace overdrive {
	namespace core {
		template <typename T>
		Logger::LogHelper Logger::operator << (const T& message) {
			LogHelper helper = info();

			helper << message;

			return helper;
		}

		template <typename T>
		Logger::LogHelper& Logger::LogHelper::operator << (const T& message) {
			mBuffer << message;

			return *this;
		}
	}
}

#include "core/logger.inl"

extern overdrive::core::Logger gLog;

#endif
