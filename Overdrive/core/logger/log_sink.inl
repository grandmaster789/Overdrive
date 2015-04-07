#pragma once

#include "log_sink.h"
#include "../../util/supports.h"

namespace overdrive {
	namespace core {
		template <typename T>
		LogSink::LogSink(T impl) :
			mWrapper(new Model<T>(std::forward<T>(impl)))
		{
		}

		template <typename T>
		LogSink::Model<T>::Model(T impl) :
			mImpl(std::forward<T>(impl))
		{
		}

		template <typename T>
		LogSink::Concept* LogSink::Model<T>::clone() const {
			return new Model<T>(mImpl);
		}

		namespace {
			// Helper structure that uses SFINAE inspection to apply an appropriate function
			template <typename T, bool tHasEquality>
			struct LogSinkCompare {
				static bool compare(
					const LogSink::Model<T>* a, 
					const LogSink::Model<T>* b
				) {
					return false; // never equal by default
				}
			};

			template <typename T>
			struct LogSinkCompare<T, true> {
				static bool compare(
					const LogSink::Model<T>* a, 
					const LogSink::Model<T>* b
				) {
					return (a->mImpl == b->mImpl); // if the underlying type supports an equality operator, use that
				}
			};
		}

		template <typename T>
		bool LogSink::Model<T>::operator==(const Concept& c) const {
			if (auto dynamic = dynamic_cast<decltype(this)>(&c))
				return LogSinkCompare<T, util::supports_equality<T>::value>::compare(this, dynamic);

			return false;
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
