#ifndef OVERDRIVE_CORE_LOGGER_H
#define OVERDRIVE_CORE_LOGGER_H

#include <mutex>
#include <string>
#include <sstream>
#include <fstream>

namespace overdrive {
	namespace core {
		class Logger {
		private:
			struct LogHelper;

		public:
			friend struct LogHelper;

			Logger(std::string filename);

			//don't do copy/move stuff
			Logger(const Logger&) = delete;
			Logger(Logger&&) = delete;
			Logger& operator = (const Logger&) = delete;
			Logger& operator = (Logger&&) = delete;

			template <typename T>
			LogHelper operator << (const T& message) {
				LogHelper helper = info();

				helper << message;
				
				return helper;
			}

			LogHelper operator << (std::ostream& (*fn)(std::ostream& os)); //overloading this allows std::iomanip functions to operate on a Logger

			LogHelper debug();		// [dbg]
			LogHelper info();		// this is the default (empty prefix)
			LogHelper warning();	// [wrn]
			LogHelper error();		// [** ERROR **]
			LogHelper fatal();		// [<=== FATAL ===>]

		private:
			void flush(std::string message);

			// Helper struct to accumulate message; flushes at the parent object when it goes out of scope
			// Move-only operations
			struct LogHelper {
				LogHelper(Logger* parent);
				LogHelper(const LogHelper&) = delete;
				LogHelper(LogHelper&& lh);
				~LogHelper();

				LogHelper& operator = (const LogHelper&) = delete;
				LogHelper& operator = (LogHelper&& lh);

				void release();

				template <typename T>
				LogHelper& operator << (const T& message) {
					mBuffer << message;

					return *this;
				}

				LogHelper& operator << (std::ostream& (*fn)(std::ostream& os));

				std::ostringstream mBuffer;
				Logger* mParent;
			};

			std::mutex mMutex;
			std::ofstream mOutputFile;
		};
	}
}

extern overdrive::core::Logger gLog;

#endif
