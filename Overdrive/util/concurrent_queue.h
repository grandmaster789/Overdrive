#ifndef OVERDRIVE_UTIL_CONCURRENTQUEUE_H
#define OVERDRIVE_UTIL_CONCURRENTQUEUE_H

#include <deque>
#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace overdrive {
	namespace core {
		class TaskProcessor;
	}

	namespace util {
		// Threadsafe wrapper around std::deque
		// (the wrapped type needs to provide a default constructor)
		template <typename T>
		class ConcurrentQueue {
		private:
			typedef boost::mutex Mutex;
			typedef boost::lock_guard<Mutex> ScopedLock;
			typedef boost::unique_lock<Mutex> UniqueLock;
			typedef boost::condition_variable Condition;

		public:
			ConcurrentQueue();
			~ConcurrentQueue();

			// override implicit functions (this class is move-only)
			ConcurrentQueue(const ConcurrentQueue&) = delete;
			ConcurrentQueue(ConcurrentQueue&& cq);
			ConcurrentQueue& operator = (const ConcurrentQueue&) = delete;
			ConcurrentQueue& operator = (ConcurrentQueue&& cq);
			void swap(ConcurrentQueue& cq);

			void push(T item);
			T pop(); //will return a default constructed T if the stopping flag was set
			bool tryPop(T& result); //returns false if it would block

			void stop();
			
		private:
			friend class core::TaskProcessor;

			mutable Mutex mMutex;
			Condition mCondition;
			std::deque<T> mInternalQueue;
			bool mStopping = false;
		};

		// IMPLEMENTATION
		template <typename T>
		ConcurrentQueue<T>::ConcurrentQueue() {
		}

		template <typename T>
		ConcurrentQueue<T>::~ConcurrentQueue() {
			stop();
		}

		template <typename T>
		ConcurrentQueue<T>::ConcurrentQueue(ConcurrentQueue&& cq) {
			swap(cq);
		}
		
		template <typename T>
		ConcurrentQueue<T>& ConcurrentQueue<T>::operator = (ConcurrentQueue&& cq) {
			ScopedLock myLock(mMutex);
			ScopedLock theirLock(cq.mMutex);

			mInternalQueue = std::move(cq.mInternalQueue);
			mStopping = cq.mStopping;

			return *this;
		}

		template <typename T>
		void ConcurrentQueue<T>::swap(ConcurrentQueue& cq) {
			using std::swap;

			ScopedLock myLock(mMutex);
			ScopedLock theirLock(cq.mMutex);
			
			swap(mInternalQueue, cq.mInternalQueue);
			swap(mStopping, cq.mStopping);
		}

		template <typename T>
		void ConcurrentQueue<T>::push(T item) {
			ScopedLock lock(mMutex);
			mInternalQueue.push_back(std::move(item));
			mCondition.notify_one();
		}

		template <typename T>
		T ConcurrentQueue<T>::pop() {
			UniqueLock lock(mMutex);

			while (mInternalQueue.empty() && !mStopping)
				mCondition.wait(lock);

			if (mStopping)
				return T();

			T result = std::move_if_noexcept(mInternalQueue.front());
			mInternalQueue.pop_front();

			return result;
		}

		template <typename T>
		bool ConcurrentQueue<T>::tryPop(T& result) {
			ScopedLock lock(mMutex);

			if (mInternalQueue.empty())
				return false;

			result = std::move_if_noexcept(mInternalQueue.front());
			mInternalQueue.pop_front();

			return true;
		}
		template <typename T>
		void ConcurrentQueue<T>::stop() {
			mStopping = true;
			mCondition.notify_all();
		}
	}
}

#endif
