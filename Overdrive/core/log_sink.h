#pragma once

#include "log_message.h"
#include <memory>

namespace overdrive {
	namespace core {
		class LogSink {
		public:
			template <typename T>
			LogSink(T impl);
			
			LogSink(const LogSink& sink);
			LogSink& operator= (const LogSink& sink);
			LogSink& operator= (LogSink&& sink);
			

			bool operator == (const LogSink& sink) const;

			void write(
				const LogMessage::Meta& meta,
				const std::string& message
			) const;

		private:
			struct Concept {
				virtual ~Concept() = default;
				virtual Concept* clone() const = 0;

				virtual void write(
					const LogMessage::Meta& meta,
					const std::string& message
				) const = 0;
			};

			template <typename T>
			struct Model : Concept {
				Model(T impl);

				virtual Concept* clone() const  override;

				virtual void write(
					const LogMessage::Meta& meta,
					const std::string& message
				) const override;

				T mImpl;
			};

			std::unique_ptr<Concept> mWrapper;
		};

		LogSink makeConsoleSink();
		LogSink makeFileSink(const std::string& filename);
	}
}

#include "log_sink.inl"