#include "preprocessor.h" // for OVERDRIVE_DEBUG
#include "core/logger.h"
#include <chrono>
#include <ctime>
#include <iostream>

overdrive::core::Logger gLog("overdrive.log"); // ze global

namespace overdrive {
	namespace core {
		Logger::Logger(std::string filename):
			mOutputFile{ std::move(filename) }
		{
			mOutputFile.sync_with_stdio(false);
		}

		Logger::LogHelper Logger::operator << (std::ostream& (*fn)(std::ostream& os)) {
			LogHelper helper(this);

			fn(helper.mBuffer);

			return helper;
		}

		std::string clockString(const std::chrono::system_clock::time_point& tp) {
			auto timeStruct = std::chrono::system_clock::to_time_t(tp);

			#pragma warning(push)
			#pragma warning(disable: 4996)
				std::string result = std::ctime(&timeStruct); // it's a little verbose, but it's also easy
			#pragma warning(pop)

			result.resize(result.size() - 1); //get rid of the trailing newline

			return result;
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
			if (mParent) {
				mBuffer << "\n";	// automatically add a newline
				mParent->flush(mBuffer.str());
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