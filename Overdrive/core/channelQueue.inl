#ifndef OVERDRIVE_CORE_CHANNELQUEUE_INL
#define OVERDRIVE_CORE_CHANNELQUEUE_INL

#include "core/channelqueue.h"

namespace overdrive {
	namespace core {
		namespace detail {
			template <typename tMessage>
			ChannelQueue<tMessage>& ChannelQueue<tMessage>::instance() {
				static ChannelQueue anInstance;
				return anInstance;
			}

			template <typename tMessage>
			template <typename tHandler>
			void ChannelQueue<tMessage>::add(tHandler* handler) {
				ScopedLock lock(mMutex);

				mHandlers.push_back(createHandler(handler));
				mOriginalPtrs.push_back(handler);
			}

			template <typename tMessage>
			template <typename tHandler>
			void ChannelQueue<tMessage>::remove(tHandler* handler) {
				ScopedLock lock(mMutex);

				auto it = std::find(mOriginalPtrs.begin(), mOriginalPtrs.end(), handler);

				if (it == mOriginalPtrs.end())
					throw std::runtime_error("Tried to remove a handler that is not in the list");

				auto idx = (it - mOriginalPtrs.begin());

				mHandlers.erase(mHandlers.begin() + idx);
				mOriginalPtrs.erase(it);
			}

			template <typename tMessage>
			void ChannelQueue<tMessage>::broadcast(const tMessage& message) {
				std::vector<Handler> localVector(mHandlers.size());

				{
					ScopedLock lock(mMutex);
					localVector = mHandlers;
				}

				for (const auto& handler : localVector)
					handler(message);
			}

			template <typename tMessage>
			ChannelQueue<tMessage>::ChannelQueue() {
			}

			template <typename tMessage>
			template <typename tHandler>
			typename ChannelQueue<tMessage>::Handler ChannelQueue<tMessage>::createHandler(tHandler* handler) {
				return [handler](const tMessage& message) { (*handler)(message); };
			}
		}
	}
}

#endif