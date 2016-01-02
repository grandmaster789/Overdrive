#pragma once

#include "channel_queue.h"
#include <algorithm>

namespace overdrive {
	namespace core {
		template <typename T>
		ChannelQueue<T>::ChannelQueue() {
		}

		template <typename T>
		ChannelQueue<T>& ChannelQueue<T>::instance() {
			static ChannelQueue result;
			return result;
		}

		template <typename T>
		template <typename U>
		void ChannelQueue<T>::add(U* handler) {
			ScopedLock lock(mMutex);

			mHandlers.push_back([handler](const T& message) { (*handler)(message); });
			mOriginalPointers.push_back(handler);
		}

		template <typename T>
		template <typename U>
		void ChannelQueue<T>::remove(U* handler) {
			using namespace std;

			ScopedLock lock(mMutex);

			auto it = find(begin(mOriginalPointers), end(mOriginalPointers), handler);
			if (it == end(mOriginalPointers))
				throw std::runtime_error("Tried to remove a handler that was not registered");

			auto idx = it - begin(mOriginalPointers);

			mHandlers.erase(mHandlers.begin() + idx);
			mOriginalPointers.erase(it);
		}

		template <typename T>
		void ChannelQueue<T>::removeAll() {
			ScopedLock lock(mMutex);

			mHandlers.clear();
			mOriginalPointers.clear();
		}

		template <typename T>
		size_t ChannelQueue<T>::getNumHandlers() const {
			ScopedLock lock(mMutex);
			return mHandlers.size();
		}

		template <typename T>
		void ChannelQueue<T>::broadcast(const T& message) {
			std::vector<Handler> localQueue;
			localQueue.reserve(mHandlers.size());

			{
				ScopedLock lock(mMutex);
				localQueue = mHandlers;
			}

			for (auto& handler: localQueue)
				handler(message);
		}
	}
}