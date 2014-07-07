#ifndef OVERDRIVE_CORE_CHANNELQUEUE_H
#define OVERDRIVE_CORE_CHANNELQUEUE_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>
#include "util/concurrent_vector.h"

namespace overdrive {
	namespace core {
		class Channel;

		namespace detail {
			template <typename tMessage>
			class ChannelQueue {
			public:
				typedef std::function<void(const tMessage&)> Handler;
				typedef std::pair<void*, Handler> HandlerPair;

				static ChannelQueue& instance() {
					static ChannelQueue anInstance;
					
					return anInstance;
				}

				template <typename tHandler>
				void add(tHandler* handler) {
					mInternalVector.push_back(
						std::make_pair(
							(void*)handler,
							createHandler(handler)
						)
					);
				}

				template <typename tHandler>
				void remove(tHandler* handler) {
					mInternalVector.remove_if(
						[handler] (const HandlerPair& pair) {
							return (handler == pair.first);
						}
					);
				}

				void broadcast(const tMessage& message) {
					auto localVector = mInternalVector.copyInternals();
					
					for (const auto& pair: localVector)
						pair.second(message);
				}

			private:
				ChannelQueue() {
				}

				template <typename tHandler>
				Handler createHandler(tHandler* handler) {
					return [handler](const tMessage& message) { (*handler)(message); };
				}

				typedef std::function<void(const tMessage&)> Handler;
				typedef std::pair<void*, Handler> HandlerPair;

				util::ConcurrentVector<HandlerPair> mInternalVector;
			};
		}
	}
}

#endif