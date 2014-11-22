#ifndef OVERDRIVE_CORE_CHANNELQUEUE_H
#define OVERDRIVE_CORE_CHANNELQUEUE_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <mutex>
#include <vector>
#include <utility>

namespace overdrive {
	namespace core {
		class Channel;

		namespace detail {
			template <typename tMessage>
			class ChannelQueue {
			public:
				typedef std::function<void(const tMessage&)> Handler;
				typedef std::lock_guard<std::mutex> ScopedLock;
				
				static ChannelQueue& instance();

				template <typename tHandler> void add(tHandler* handler);
				template <typename tHandler> void remove(tHandler* handler);

				void broadcast(const tMessage& message);

			private:
				ChannelQueue();

				template <typename tHandler> Handler createHandler(tHandler* handler);

				std::mutex mMutex;
				std::vector<Handler> mHandlers;
				std::vector<void*> mOriginalPtrs;
			};
		}
	}
}

#include "core/channelqueue.inl"

#endif