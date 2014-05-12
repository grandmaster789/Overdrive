#ifndef OVERDRIVE_UTIL_CONCURRENTQUEUE_H
#define OVERDRIVE_UTIL_CONCURRENTQUEUE_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace overdrive {
	namespace core {
		class TaskProcessor;
	}

	namespace util {
		template <typename T>
		class ConcurrentQueue {
		private:
			typedef std::mutex Mutex;
			typedef std::lock_guard<Mutex> ScopedLock;
			typedef std::unique_lock<Mutex> UniqueLock;

			friend class core::TaskProcessor;

		public:
			ConcurrentQueue() {
			}

			ConcurrentQueue(const ConcurrentQueue& cq) {
				ScopedLock lock(cq.mMutex);
				
				mInternalQueue = cq.mInternalQueue;
			}

			void push(T value) {
				ScopedLock lock(mMutex);

				mInternalQueue.push(std::move(value));
				mCondition.notify_one();
			}

			//this will wait when appropriate
			void pop(T& result) {
				UniqueLock lock(mMutex);
				
				mCondition.wait(lock, [this] { return !mInternalQueue.empty(); });
				result = std::move(mInternalQueue.front());
				mInternalQueue.pop();
			}

			//will return false if it would block
			bool tryPop(T& result) {
				ScopedLock lock(mMutex);

				if (mInternalQueue.empty())
					return false;

				result = std::move(mInternalQueue.front());
				mInternalQueue.pop();

				return true;
			}

			bool isEmpty() const {
				ScopedLock lock(mMutex);

				return mInternalQueue.empty();
			}

			std::vector<T> toVectorAndClear() {
				ScopedLock lock(mMutex);
				
				std::vector<T> result(mInternalQueue.size());	

				while (!mInternalQueue.empty()) {
					auto item = std::move(mInternalQueue.front());
					mInternalQueue.pop();
					result.emplace_back(std::move(item));
				}

				return result;
			}

		private:
			std::queue<T> mInternalQueue;
			std::mutex mMutex;
			std::condition_variable mCondition;
		};
	}
}

#endif
