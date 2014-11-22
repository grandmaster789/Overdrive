#ifndef OVERDRIVE_UTIL_CONCURRENT_VECTOR_H
#define OVERDRIVE_UTIL_CONCURRENT_VECTOR_H

#include <vector>
#include <mutex>
#include <condition_variable>

namespace overdrive {
	namespace util {
		// [Note] while this does work, it is also kinda shitty :(
		template <typename T>
		class ConcurrentVector {
		public:
			typedef std::mutex Mutex;
			typedef std::lock_guard<Mutex> ScopedLock;
			typedef std::unique_lock<Mutex> UniqueLock;
			typedef std::condition_variable Condition;

			void push_back(const T& item);
			void push_back(T&& item);
			void pop_back(T& item);

			void swap(ConcurrentVector& other);

			const std::vector<T>& getInternalsUnsafe() const; // no locking
			std::vector<T> copyInternals() const;

			void remove_if(std::function<bool(const T& value)> condition);

		private:
			std::vector<T> mInternalVector;
			mutable Mutex mMutex;
			Condition mCondition;
		};
	}
}

#include "util/concurrent_vector.inl"

#endif
