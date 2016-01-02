#pragma once

#include "log_level.h"
#include "log_message.h"
#include "log_sink.h"

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

			void add(const LogSink& sink);
			void remove(const LogSink& sink);
			void removeAll();
			size_t getSinkCount() const;

			void flush(const LogMessage* message);

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