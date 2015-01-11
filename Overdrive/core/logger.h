#pragma once

#include "log_level.h"
#include "log_message.h"

#include "util/active.h"

#include <vector>

namespace overdrive {
	namespace core {
		class LogSink;
		class LogMessage;

		class Logger {
		public:
			Logger(const std::string& filename);

			LogMessage operator()(
				eLogLevel level, 
				const std::string& filename, 
				int line
			);

			void add(const LogSink& sink);
			void remove(const LogSink& sink);

			void flush(const LogMessage& message) const;

		private:
			std::vector<LogSink> mSinks;
			std::unique_ptr<util::Active> mActive;
		};

		extern Logger gLogger;
	}
}

#define gLogLevel(level) ::overdrive::core::gLogger( \
	::overdrive::core::eLogLevel::level,             \
	__FILE__,                                        \
	__LINE__                                         \
)

#define gLog        gLogLevel(MESSAGE)

#define gLogDebug   gLogLevel(DEBUG)
#define gLogMessage gLogLevel(MESSAGE)
#define gLogError   gLogLevel(ERROR_)
#define gLogWarning gLogLevel(WARNING)
#define gLogFatal   gLogLevel(FATAL)