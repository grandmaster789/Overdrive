#pragma once

#include <ostream>
#include <sstream>
#include <string>

#include "log_level.h"

namespace overdrive {
	namespace core {
		class Logger;
		
		class LogMessage {
		private:
			friend class core::Logger;

			LogMessage(Logger* owner, eLogLevel level, const std::string& file, int line);
			LogMessage(const LogMessage&) = delete;
			LogMessage(LogMessage&& lm);

		public:
			~LogMessage();
			LogMessage& operator = (const LogMessage&) = delete;
			LogMessage& operator = (LogMessage&& lm) = delete;
		
			template <typename T>
			LogMessage& operator << (const T& value); // accept values

			LogMessage& operator << (std::ostream& (*fn)(std::ostream&)); // accept iostream manipulator functions (endl etc)

			struct Meta {
				eLogLevel mLevel;
				std::string mFile;
				int mLine;
			};

		private:
			std::ostringstream mBuffer;
			Logger* mOwner;
				
			Meta mMeta;
		};
	}
}

#include "log_message.inl"