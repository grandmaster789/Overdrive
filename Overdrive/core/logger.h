#pragma once

#include "logger/log_level.h"
#include "logger/log_message.h"
#include "logger/log_sink.h"

#include "../util/active.h"

#include <vector>

namespace overdrive {
	namespace core {
		class Logger {
		public:
			explicit Logger();
			Logger(const std::string& filename);

			LogMessage operator()(
				eLogLevel level,
				const std::string& filename,
				int line
			);

			LogMessage operator()(); // this provides some dummy metadata, really only for use in unit testing; prefer the macros

			void add(const LogSink& sink);
			void remove(const LogSink& sink); // [NOTE] implementation is pretty... exotic (because of several implicit assumptions needed to make this work)
			void removeAll();
			size_t getSinkCount() const; // pretty much only used in unit testing
			
			void flush(const LogMessage& message);

			static Logger& instance(); // provide a static, global logger as well (local loggers are still allowed)

		private:
			std::vector<LogSink> mSinks;
			std::unique_ptr<util::Active> mActive;
		};
	}
}

#define gLogLevel(level) (					\
	overdrive::core::Logger::instance()(	\
		overdrive::core::eLogLevel::level,	\
		__FILE__,							\
		__LINE__							\
	))

#define gLog        (gLogLevel(MESSAGE))

#define gLogDebug   (gLogLevel(DEBUG))
#define gLogMessage (gLogLevel(MESSAGE))
#define gLogError   (gLogLevel(ERROR_))
#define gLogWarning (gLogLevel(WARNING))
#define gLogFatal   (gLogLevel(FATAL))