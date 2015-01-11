#pragma once

#include "log_message.h"
#include <memory>

namespace overdrive {
	namespace core {
		/**
			Encapsulated polymorphism, based on Sean Parents' talks at Going Native and
			BoostCon'12. Almost everything is 'boilerplate', but doesn't seem very
			easy to generalize.
		*/
		class LogSink {
		public:
			/**
				The wrapped type T should have:
				- void operator()(const LogMessage& meta, const std::string& message) const;
			*/
			template <typename T>
			LogSink(T impl); // note that this allows implicit conversion to a LogSink

			LogSink(const LogSink& sink);
			LogSink(LogSink&&) noexcept = default;

			LogSink& operator= (const LogSink& sink) noexcept;
			LogSink& operator= (LogSink&&) noexcept = default;
			
			bool operator == (const LogSink& sink) const;

			void forward(
				const LogMessage::Meta& meta,
				const std::string& message
			) const;

		private:
			struct Concept {
				virtual ~Concept() = default;
				virtual Concept* clone() const = 0;
				
				virtual void forward(
					const LogMessage::Meta& meta,
					const std::string& message
				) const = 0;
			};

			template <typename T>
			struct Model : Concept {
				Model(T impl);

				virtual Concept* clone() const  override;

				virtual void forward(
					const LogMessage::Meta& meta, 
					const std::string& message
				) const override;

				T mImpl;
			};

			std::unique_ptr<const Concept> mWrapper;
		};

		LogSink makeConsoleSink();
		LogSink makeFileSink(const std::string& filename);

		
	}
}

#include "log_sink.inl"