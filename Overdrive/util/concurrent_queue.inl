#ifndef OVERDRIVE_UTIL_CONCURRENTQUEUE_INL
#define OVERDRIVE_UTIL_CONCURRENTQUEUE_INL

#include "concurrent_queue.h"

namespace overdrive {
	namespace util {
		template <typename T>
		ConcurrentQueue<T>::ConcurrentQueue() {
		}
		
		template <typename T>
		void ConcurrentQueue<T>::push(const T& item) {
			{
				ScopedLock lock(mMutex);
				mInternalQueue.push(item);
			}

			mCondition.notify_one();
		}

		template <typename T>
		void ConcurrentQueue<T>::push(T&& item) {
			{
				ScopedLock lock(mMutex);
				mInternalQueue.push(std::forward<T>(item));
			}

			mCondition.notify_one();
		}

		template <typename T>
		T ConcurrentQueue<T>::pop() {
			UniqueLock lock(mMutex);

			while (mInternalQueue.empty())
				mCondition.wait(lock);

			auto result = std::move(mInternalQueue.front());
			mInternalQueue.pop();
			return result;
		}

		template <typename T>
		T ConcurrentQueue<T>::pop_unsafe() {
			auto result = std::move(mInternalQueue.front());
			mInternalQueue.pop();
			return result;
		}

		template <typename T>
		void ConcurrentQueue<T>::pop(T& result) {
			UniqueLock lock(mMutex);

			while (mInternalQueue.empty())
				mCondition.wait(lock);

			result = std::move(mInternalQueue.front());
			mInternalQueue.pop();
		}

		template <typename T>
		bool ConcurrentQueue<T>::try_pop(T& result) {
			ScopedLock lock(mMutex);

			if (mInternalQueue.empty())
				return false;
				
			result = std::move(mInternalQueue.front());
			mInternalQueue.pop();

			return result;
		}

		template <typename T>
		bool ConcurrentQueue<T>::isEmpty() const {
			ScopedLock lock(mMutex);
			return mInternalQueue.empty();
		}

		template <typename T>
		bool ConcurrentQueue<T>::isEmpty_unsafe() const {
			return mInternalQueue.empty();
		}

		template <typename T>
		void ConcurrentQueue<T>::swap(ConcurrentQueue& cq) {
			ScopedLock lock_other(cq.mMutex);
			ScopedLock lock_self(mMutex);

			std::swap(cq.mInternalQueue, mInternalQueue);
		}
	}
}

#endif
