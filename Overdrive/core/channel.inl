#pragma once

#include "channel.h"
#include "channel_queue.h"

namespace overdrive {
	namespace core {
		template <typename T, typename U>
		void Channel::add(U* handler) {
			ChannelQueue<T>::instance().add(handler);
		}

		template <typename T, typename U>
		void Channel::remove(U* handler) {
			ChannelQueue<T>::instance().remove(handler);
		}

		template <typename T>
		void Channel::removeAll() {
			ChannelQueue<T>::instance().removeAll();
		}

		template <typename T>
		size_t Channel::getNumHandlers() {
			return ChannelQueue<T>::instance().getNumHandlers();
		}

		template <typename T>
		void Channel::broadcast(const T& message) {
			ChannelQueue<T>::instance().broadcast(message);
		}
	}

	template <typename T>
	MessageHandler<T>::MessageHandler() {
		core::Channel::add<T>(this);
	}

	template <typename T>
	MessageHandler<T>::~MessageHandler() {
		core::Channel::remove<T>(this);
	}
}
