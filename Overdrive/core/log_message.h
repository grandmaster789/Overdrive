#pragma once

#include "log_level.h"

#include <string>
#include <sstream>
#include <ostream>

namespace overdrive {
	namespace core {
		class Logger;
		class LogSink;

		class LogMessage {
		private:
			friend class Logger;
			friend class LogSink;

			LogMessage(
				eLogLevel level,
				const std::string& file,
				int line,
				Logger* owner
			);

		public:
			~LogMessage();

			LogMessage(LogMessage&& message);

			template <typename T>
			LogMessage& operator << (const T& message);
			LogMessage& operator << (std::ostream& (*fn)(std::ostream& os));

			struct Meta {
				eLogLevel mLevel;
				std::string mFile;
				int mLine;
			};

		private:
			Meta mMeta;
			Logger* mOwner; // the spawning object is guaranteed to outlive this one, so it's fine to use a raw pointer here
			std::ostringstream mBuffer;
		};
	}
}

#include "log_message.inl"