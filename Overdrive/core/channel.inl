#ifndef OVERDRIVE_CORE_CHANNEL_INL
#define OVERDRIVE_CORE_CHANNEL_INL

#include "core/channel.h"

namespace overdrive {
	namespace core {
		// ------------------------------ Channel -----------------------------
		template <typename tEvent, class tHandler>
		void Channel::add(tHandler* handler) {
			detail::ChannelQueue<tEvent>::instance().add(handler);
		}

		template <typename tEvent, class tHandler>
		void Channel::remove(tHandler* handler) {
			detail::ChannelQueue<tEvent>::instance().remove(handler);
		}

		template <typename tEvent>
		void Channel::broadcast(const tEvent& message) {
			detail::ChannelQueue<tEvent>::instance().broadcast(message);
		}

		// ------------------------------ MessageHandler ------------------------
		template <typename T>
		MessageHandler<T>::MessageHandler() {
			Channel::add<T>(this);
		}

		template <typename T>
		MessageHandler<T>::~MessageHandler() {
			Channel::remove<T>(this);
		}
	}
}

#endif
