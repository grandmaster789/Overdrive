#pragma once

#include "log_message.h"

namespace overdrive {
	namespace core {
		template <typename T>
		LogMessage& LogMessage::operator<<(const T& value) {
			mBuffer << value;
			return *this;
		}
	}
}
