#pragma once

#include <vector>
#include <mutex>

namespace overdrive {
	namespace core {
		template <typename tMessage>
		class ChannelQueue {
		private:
			ChannelQueue();

		public:
			static ChannelQueue& instance();

			template <typename tHandler>
			void add(tHandler* handler);

			template <typename tHandler>
			void remove(tHandler* handler);
			void removeAll();

			size_t getNumHandlers() const;

			void broadcast(const tMessage& message);

		private:
			typedef std::function<void(const tMessage&)> Handler;
			typedef std::mutex Mutex;
			typedef std::lock_guard<Mutex> ScopedLock;
			
			std::mutex mMutex;
			std::vector<Handler> mHandlers;
			std::vector<void*> mOriginalPointers;
		};
	}
}

#include "channel_queue.inl"