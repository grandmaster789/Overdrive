#pragma once

namespace overdrive {
	namespace core {
		struct Channel {
			template <typename tMessage, typename tHandler>
			static void add(tHandler* handler);

			template <typename tMessage, typename tHandler>
			static void remove(tHandler* handler);

			template <typename tMessage>
			static void removeAll();

			template <typename tMessage>
			static size_t getNumHandlers();

			template <typename tMessage>
			static void broadcast(const tMessage& message);
		};
	}

	// [NOTE] This *can* be used as a base class, but it is not required
	template <typename T>
	class MessageHandler {
	public:
		MessageHandler();
		~MessageHandler();

		virtual void operator()(const T& message) = 0;
	};
}

#include "channel.inl"