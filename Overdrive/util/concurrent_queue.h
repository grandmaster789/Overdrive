#ifndef OVERDRIVE_UTIL_CONCURRENTQUEUE_H
#define OVERDRIVE_UTIL_CONCURRENTQUEUE_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace overdrive {
	namespace util {
		template <typename T>
		class ConcurrentQueue {
		private:
			typedef std::mutex Mutex;
			typedef std::lock_guard<Mutex> ScopedLock;
			typedef std::unique_lock<Mutex> UniqueLock;
			typedef std::condition_variable Condition;

		public:
			ConcurrentQueue() {}
			ConcurrentQueue(const ConcurrentQueue&) = delete;
			ConcurrentQueue& operator = (const ConcurrentQueue&) = delete;

			void push(const T& item) {
				{
					ScopedLock lock(mMutex);
					mInternalQueue.push(item);
				}

				mCondition.notify_one();
			}

			void push(T&& item) {
				{
					ScopedLock lock(mMutex);
					mInternalQueue.push(std::move(item));
				}

				mCondition.notify_one();
			}

			// Easy to use, slightly non-exception-safe but threadsafe
			T pop() {
				UniqueLock lock(mMutex);
				
				while (mInternalQueue.empty())
					mCondition.wait(lock);

				auto result = std::move(mInternalQueue.front());
				mInternalQueue.pop();
				return result;
			}

			// The not-so-threadsafe variant (can be safely used when the queue is being consumed completely in a while loop)
			T pop_unsafe() {
				auto result = std::move(mInternalQueue.front());
				mInternalQueue.pop();
				return result;
			}

			// This is the slightly more exception-safe variant, also threadsafe
			void pop(T& result) {
				UniqueLock lock(mMutex);
				
				while (mInternalQueue.empty())
					mCondition.wait(lock);

				result = std::move(mInternalQueue.front());
				mInternalQueue.pop();
			}

			// This will return false if it would block
			bool try_pop(T& result) {
				ScopedLock lock(mMutex);
				
				if (mInternalQueue.empty())
					return false;

				result = std::move(mInternalQueue.front());
				mInternalQueue.pop();

				return result;
			}

			bool isEmpty() const {
				ScopedLock lock(mMutex);
				return mInternalQueue.empty();
			}

			bool isEmpty_unsafe() const {
				return mInternalQueue.empty();
			}

			void swap(ConcurrentQueue& cq) {
				ScopedLock lock_other(cq.mMutex);
				ScopedLock lock_self(mMutex);
				
				std::swap(cq.mInternalQueue, mInternalQueue);
			}

		private:
			Mutex mMutex;
			Condition mCondition;

			std::queue<T> mInternalQueue;
		};
	}
}

#endif
