#include "stdafx.h"
#include "logger.h"
#include "log_sink.h"

namespace overdrive {
	namespace core {
		Logger::Logger(const std::string& filename) {
			mActive = util::Active::create();

			add(makeConsoleSink());
			add(makeFileSink(filename));
		}

		LogMessage Logger::operator()(
			eLogLevel level,
			const std::string& filename,
			int line
		) {
			return LogMessage(
				level, 
				filename, 
				line, 
				this
			);
		}

		void Logger::add(const LogSink& sink) {
			mSinks.push_back(sink); // perhaps check for duplicates?
		}

		void Logger::remove(const LogSink& sink) {
			auto it = std::find(mSinks.begin(), mSinks.end(), sink);

			if (it == mSinks.end())
				throw std::runtime_error("Tried to remove a sink that was not added yet");

			mSinks.erase(it);
		}

		void Logger::flush(const LogMessage& message) const {
			/*
			// This is the single-threaded version

			auto msg = message.mBuffer.str();
			
			for (auto&& sink: mSinks)
				sink.forward(message.mMeta, msg);
			*/

			// This is the Active Object (and threadsafe) version

			auto&& sinks = mSinks;
			auto&& meta = message.mMeta;
			auto msg = message.mBuffer.str();

			mActive->send([=] {
				for (auto&& sink : sinks)
					sink.forward(meta, msg);
			});
		}

		Logger gLogger("overdrive.log");
	}
}
