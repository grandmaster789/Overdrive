#include "preprocessor.h" // for OVERDRIVE_DEBUG
#include "core/logger.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>

overdrive::core::Logger gLog("overdrive.log"); // ze global

namespace overdrive {
	namespace core {
		Logger::Logger(std::string filename):
			mOutputFile{ std::move(filename) }
		{
			mOutputFile.sync_with_stdio(false); // a static call -- the first Logger will turn off stdio sync for everything!
		}

		Logger::LogHelper Logger::operator << (std::ostream& (*fn)(std::ostream& os)) {
			LogHelper helper(this);

			fn(helper.mBuffer);

			return helper;
		}

		std::string clockString(const std::chrono::system_clock::time_point& tp) {
			auto timeStruct = std::chrono::system_clock::to_time_t(tp);

			// converting a time_t to a string is one of those locale-influenced thingies
			// and is affected by imbued iostreams. However, I'm not really inclined to
			// use anything other than the system default. 
			// for more information (and formatting options) search std::put_time @ cppreference
			// [Note] This is the C-library, so the microsoft implementation is *not* up to date
			//	in the documentation, assume that if it says C++11 it won't work...

			std::stringstream sstr;

			#pragma warning(push)
			#pragma warning(disable: 4996) // the call to 'localtime' is 'unsafe'...
				sstr << std::put_time(std::localtime(&timeStruct), "%Y/%m/%d %H:%M:%S"); // year/month/day hour/minutes/seconds
			#pragma warning(pop)

			return sstr.str();
		}

		Logger::LogHelper::LogHelper(Logger* parent):
			mParent(parent)
		{
			mBuffer.sync_with_stdio(false);
			mBuffer << clockString(std::chrono::system_clock::now());
		}

		Logger::LogHelper::LogHelper(LogHelper&& lh):
			mParent(std::move(lh.mParent)),
			mBuffer(std::move(lh.mBuffer))
		{
			lh.release(); // the old one doesn't have to flush anymore
		}

		Logger::LogHelper::~LogHelper() {
			try {
				if (mParent) {
					mBuffer << "\n";	// automatically add a newline
					mParent->flush(mBuffer.str());
				}
			}
			catch (...) {
				std::cerr << "Failed to flush to log!\n";
			}
		}

		Logger::LogHelper& Logger::LogHelper::operator= (LogHelper&& lh) {
			mParent = std::move(lh.mParent);
			mBuffer = std::move(lh.mBuffer);

			lh.release();

			return *this;
		}

		void Logger::LogHelper::release() {
			mParent = nullptr;
		}

		Logger::LogHelper& Logger::LogHelper::operator << (std::ostream& (*fn)(std::ostream& os)) {
			fn(mBuffer);
			return *this;
		}

		void Logger::flush(std::string message) {
			std::lock_guard<std::mutex> lock(mMutex);

			mOutputFile << message;

			#ifdef OVERDRIVE_DEBUG
				std::clog << message;
			#endif
		}

		Logger::LogHelper Logger::debug() {
			LogHelper helper(this);

			helper << " [dbg] ";

			return helper;
		}

		Logger::LogHelper Logger::info() {
			LogHelper helper(this);

			helper << "       ";

			return helper;
		}

		Logger::LogHelper Logger::warning() {
			LogHelper helper(this);

			helper << " [wrn] ";

			return helper;
		}

		Logger::LogHelper Logger::error() {
			LogHelper helper(this);

			helper << " [** ERROR **] ";

			return helper;
		}

		Logger::LogHelper Logger::fatal() {
			LogHelper helper(this);

			helper << " [<=== FATAL ===>] ";

			return helper;
		}
	}
}