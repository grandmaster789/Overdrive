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
				- Must be copyable
				- void operator()(const LogMessage& meta, const std::string& message) const;
				- (optional) bool operator==(const T&) const; // if this is missing, the created sink cannot be found/removed after adding it to a logger backend
			*/
			template <typename T>
			LogSink(T impl); // note that this allows implicit conversion to a LogSink

			LogSink(const LogSink& sink);

			LogSink& operator= (const LogSink& sink);

			bool operator == (const LogSink& sink) const;

			void write(
				const LogMessage::Meta& meta,
				const std::string& message
			) const;

		private:
			struct Concept {
				virtual ~Concept() = default;
				virtual Concept* clone() const = 0;
				virtual bool operator == (const Concept& c) const = 0;

				virtual void write(
					const LogMessage::Meta& meta,
					const std::string& message
				) const = 0;
			};

		public:
			template <typename T>
			struct Model : Concept {
				Model(T impl);

				virtual Concept* clone() const  override;
				virtual bool operator == (const Concept& c) const;

				virtual void write(
					const LogMessage::Meta& meta,
					const std::string& message
				) const override;

				T mImpl;
			};

		private:
			std::unique_ptr<Concept> mWrapper;
		};

		// [NOTE] The console sink does not support equality, and can thus not be found/removed after adding it to a Logger
		// [NOTE] Lambda's support (), but not == and thus cannot be removed after adding them to a logger backend

		LogSink makeConsoleSink();							// formatting is: [lvl] message [newline]
		LogSink makeFileSink(const std::string& filename);	// formatting is: [timestamp] [lvl] message (sourcefile:linenumber) [newline]
		LogSink makeStringSink(std::string& buffer);		// formatting is: message [newline]			NOTE: The message is appended to the string
	}
}

#include "log_sink.inl"