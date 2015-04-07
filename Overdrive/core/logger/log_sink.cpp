#include "stdafx.h"
#include "log_sink.h"

#include <iostream>
#include <memory>
#include <string>
#include <fstream>

#include <chrono>
#include <iomanip>
#include <ctime>

namespace overdrive {
	namespace core {
		LogSink::LogSink(const LogSink& sink) :
			mWrapper(sink.mWrapper->clone())
		{
		}

		LogSink& LogSink::operator = (const LogSink& sink) {
			mWrapper.reset(sink.mWrapper->clone());
			return *this;
		}

		bool LogSink::operator == (const LogSink& sink) const {
			return ((*mWrapper) == (*sink.mWrapper));
		}

		void LogSink::write(
			const LogMessage::Meta& meta,
			const std::string& message
		) const {
			mWrapper->write(meta, message);
		}

		/******  Console Sink *****/
		LogSink makeConsoleSink() {
			return [](
				const LogMessage::Meta& meta,
				const std::string& message
			) {
				std::cout
					<< meta.mLevel
					<< message
					<< "\n";
			};
		}

		/***** File Sink (with timestamps) *****/
		namespace {
			struct FileSink {
				FileSink(const std::string& filename) :
					mFile(std::make_shared<std::ofstream>(filename)), // an ofstream is not copyable, so we must use shared_ptr to treat it as such anyway
					mFilename(filename)
				{
					if (!mFile->good()) {
						std::string message = "Failed to open file sink: ";
						message.append(filename);
 
						throw std::runtime_error(message);
					}
				}

				void operator()(
					const LogMessage::Meta& meta,
					const std::string& message
				) const {
					using namespace std::chrono;

					auto now = system_clock::now();
					auto time_t = system_clock::to_time_t(now);
					auto local_time = std::localtime(&time_t);

					(*mFile)
						<< std::put_time(local_time, "[%H:%M:%S] ")
						<< meta.mLevel
						<< message
						<< " ("
						<< meta.mFile
						<< ":"
						<< meta.mLine
						<< ")\n";
				}

				bool operator== (const FileSink& sink) const {
					return mFilename == sink.mFilename;
				}

				std::string mFilename;
				std::shared_ptr<std::ofstream> mFile;
			};
		}

		LogSink makeFileSink(const std::string& filename) {
			return FileSink(filename);
		}

		/***** String sink (append) *****/
		/*
		// While the lambda works most of the time, it doesn't supply an == operator,
		// so it can't be removed afterwards.
		LogSink makeStringSink(std::string& buffer) {
			return [&buffer](
				const LogMessage::Meta&,
				const std::string& message
			) {
				buffer.append(message);
				buffer.append("\n");
			};
		}
		*/

		namespace {
			struct StringSink {
				StringSink(std::string* buffer) :
					mBuffer(buffer)
				{
				}

				void operator()(
					const LogMessage::Meta& meta,
					const std::string& message
				) const {
					mBuffer->append(message);
					mBuffer->append("\n");
				}

				bool operator == (const StringSink& sink) const {
					return mBuffer == sink.mBuffer;
				}

				std::string* mBuffer;
			};
		}
		
		LogSink makeStringSink(std::string& buffer) {
			return StringSink(&buffer);
		}
	}
}