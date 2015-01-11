#pragma once

#include "log_sink.h"

namespace overdrive {
	namespace core {
		template <typename T>
		LogSink::LogSink(T impl):
			mWrapper(new Model<T>(std::forward<T>(impl)))
		{
		}

		template <typename T>
		LogSink::Model<T>::Model(T impl):
			mImpl(std::forward<T>(impl))
		{
		}

		template <typename T>
		LogSink::Concept* LogSink::Model<T>::clone() const {
			return new Model<T>(mImpl);
		}

		template <typename T>
		void LogSink::Model<T>::forward(
			const LogMessage::Meta& meta,
			const std::string& message
		) const {
			mImpl(meta, message);
		}
	}
}
