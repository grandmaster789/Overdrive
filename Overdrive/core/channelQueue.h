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
				
				static ChannelQueue& instance() {
					static ChannelQueue anInstance;
					
					return anInstance;
				}

				template <typename tHandler>
				void add(tHandler* handler) {
					ScopedLock lock(mMutex);

					mHandlers.push_back(createHandler(handler));
					mOriginalPtrs.push_back(handler);
				}

				template <typename tHandler>
				void remove(tHandler* handler) {
					ScopedLock lock(mMutex);

					auto it = std::find(mOriginalPtrs.begin(), mOriginalPtrs.end(), handler);
					
					if (it == mOriginalPtrs.end())
						throw std::runtime_error("Tried to remove a handler that is not in the list");

					auto idx = (it - mOriginalPtrs.begin());

					mHandlers.erase(mHandlers.begin() + idx);
					mOriginalPtrs.erase(it);
				}

				void broadcast(const tMessage& message) {
					std::vector<Handler> localVector(mHandlers.size());

					{
						ScopedLock lock(mMutex);
						localVector = mHandlers;
					}
					
					for (const auto& handler : localVector)
						handler(message);
				}

			private:
				ChannelQueue() {
				}

				template <typename tHandler>
				Handler createHandler(tHandler* handler) {
					return [handler](const tMessage& message) { (*handler)(message); };
				}

				std::mutex mMutex;
				std::vector<Handler> mHandlers;
				std::vector<void*> mOriginalPtrs;
			};
		}
	}
}

#endif