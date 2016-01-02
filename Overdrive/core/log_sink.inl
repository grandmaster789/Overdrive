#pragma once

#include "log_sink.h"

namespace overdrive {
	namespace core {
		template <typename T>
		LogSink::LogSink(T impl) :
			mWrapper(new Model<T>(std::move(impl)))
		{
		}

		template <typename T>
		LogSink::Model<T>::Model(T impl) :
			mImpl(std::move(impl))
		{
		}

		template <typename T>
		LogSink::Concept* LogSink::Model<T>::clone() const {
			return new Model<T>(mImpl);
		}

		template <typename T>
		void LogSink::Model<T>::write(
			const LogMessage::Meta& meta,
			const std::string& message
		) const {
			mImpl(meta, message);
		}
	}
}
