#ifndef OVERDRIVE_UTIL_CONCURRENTQUEUE_H
#define OVERDRIVE_UTIL_CONCURRENTQUEUE_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace overdrive {
	namespace util {
		// [Note] While this works it's still a queue, which is shitty
		template <typename T>
		class ConcurrentQueue {
		private:
			typedef std::mutex Mutex;
			typedef std::lock_guard<Mutex> ScopedLock;
			typedef std::unique_lock<Mutex> UniqueLock;
			typedef std::condition_variable Condition;

		public:
			ConcurrentQueue();
			ConcurrentQueue(const ConcurrentQueue&) = delete;
			ConcurrentQueue& operator = (const ConcurrentQueue&) = delete;

			void push(const T& item);
			void push(T&& item);

			T pop(); // Easy to use, slightly non-exception-safe but still threadsafe
			T pop_unsafe(); // The not-so-threadsafe variant (can be safely used when the queue is being consumed completely in a while loop)
			void pop(T& result); // This is the more exception-safe variant, also threadsafe
			bool try_pop(T& result); // This will return false if it would block

			bool isEmpty() const;
			bool isEmpty_unsafe() const;

			void swap(ConcurrentQueue& cq);

		private:
			Mutex mMutex;
			Condition mCondition;

			std::queue<T> mInternalQueue;
		};
	}
}

#include "util/concurrent_queue.inl"

#endif
