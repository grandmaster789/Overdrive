#ifndef OVERDRIVE_CORE_CHANNEL_H
#define OVERDRIVE_CORE_CHANNEL_H

#include "core/channelqueue.h"
#include "util/unique_resource.h"

namespace overdrive {
	namespace core {
		class Channel {
		public:
			template <typename tEvent, class tHandler>
			static void add(tHandler* handler) {
				detail::ChannelQueue<tEvent>::instance().add(handler);
			}

			template <typename tEvent, class tHandler>
			static void remove(tHandler* handler) {
				detail::ChannelQueue<tEvent>::instance().remove(handler);
			}

			template <typename tEvent>
			static void broadcast(const tEvent& message) {
				detail::ChannelQueue<tEvent>::instance().broadcast(message);
			}
		};

		// This is the contract that an object should fulfill for correct usage with a Channel
		// Note that this is not an actual required base class, even though it can be used as one
		template <typename tMessage>
		class MessageHandler {
		public:
			MessageHandler() {
				Channel::add<tMessage>(this);
			}

			MessageHandler(const MessageHandler& ) {
				Channel::add<tMessage>(this); 
			}

			virtual ~MessageHandler() {
				Channel::remove<tMessage>(this);
			}

			virtual void operator()(const tMessage&) = 0;
		};
	}
}

#endif
