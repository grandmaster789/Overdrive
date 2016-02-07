#pragma once

#include "spinlock.h"
#include <deque>
#include <condition_variable>

namespace overdrive {
	namespace util {
		// [NOTE] possibly blocking operations were left out intentionally (pop_front, pop_back)
		template <typename T>
		class ThreadsafeQueue {
		public:
			ThreadsafeQueue(const ThreadsafeQueue&) = delete;
			ThreadsafeQueue& operator = (const ThreadsafeQueue&) = delete;

			void pushFront(const T& value);
			void pushBack(const T& value);

			bool tryPopFront(T& value); // returns false if it would block
			bool tryPopBack(T& value);

			void clear();
			size_t size() const;
			bool empty() const;

		private:
			mutable Spinlock mMutex;
			typedef std::lock_guard<Spinlock> ScopedLock;

			std::deque<T> mInternalQueue;
		};
	}
}

#include "threadsafeQueue.inl"