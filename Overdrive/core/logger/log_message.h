#pragma once

#include "log_level.h"

#include <string>
#include <sstream>
#include <ostream>

namespace overdrive {
	namespace core {
		class Logger;
		class LogSink;

		/*
			Collects possibly multiple parts of a message, stores some metadata and flushes the message
			upon destruction.
		*/
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
			~LogMessage(); // flush the message upon destruction

			LogMessage(LogMessage&& message); // if the message is moved, the husk doesn't need to flush

			template <typename T>
			LogMessage& operator << (const T& message);
			LogMessage& operator << (std::ostream& (*fn)(std::ostream& os));

			// message attributes
			struct Meta {
				eLogLevel mLevel;
				std::string mFile;	// intended to store __FILE__
				int mLine;			// intended to store __LINE__
			};

		private:
			Meta mMeta;
			Logger* mOwner; // the spawning object is guaranteed to outlive this one, so it's fine to use a raw pointer here
			std::ostringstream mBuffer;
		};
	}
}

#include "log_message.inl"