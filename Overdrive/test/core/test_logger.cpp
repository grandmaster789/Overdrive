#include "core/logger.h"

namespace overdrive {
	namespace test {
		namespace core {
			void testLogger() {
				int i = 5;
				float f = 9.9f;
				std::string s("test string");

				gLog << "Sample message";
				gLog << "integer: " << i;
				gLog << "float: " << f;
				gLog << "string: " << s;

				/*
				gLogSev(DEBUG) << "Debug message";
				gLogSev(INFO) << "Info message";
				gLogSev(WARNING) << "Warning message";
				gLogSev(ERROR) << "Error message";
				gLogSev(FATAL) << "Fatal message";
				*/
				gLog.debug() << "Debug message";
				gLog.info() << "Info message";
				gLog.warning() << "Warning message";
				gLog.error() << "Error message";
				gLog.fatal() << "Fatal message";
			}
		}
	}
}