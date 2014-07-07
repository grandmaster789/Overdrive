#ifndef OVERDRIVE_UTIL_CONCURRENT_VECTOR_H
#define OVERDRIVE_UTIL_CONCURRENT_VECTOR_H

#include <vector>
#include <mutex>
#include <condition_variable>

namespace overdrive {
	namespace util {
		template <typename T>
		class ConcurrentVector {
		public:
			typedef std::mutex Mutex;
			typedef std::lock_guard<Mutex> ScopedLock;
			typedef std::unique_lock<Mutex> UniqueLock;
			typedef std::condition_variable Condition;

			void push_back(const T& item) {
				ScopedLock lock(mMutex);
				mInternalVector.push_back(item);
				mCondition.notify_all();
			}

			void push_back(T&& item) {
				ScopedLock lock(mMutex);
				mInternalVector.push_back(std::move(item));
				mCondition.notify_all();
			}

			void pop_back(T& item) {
				UniqueLock lock(mMutex);

				mCondition.wait(lock, [&] { return !mInternalVector.empty(); });

				if (!mInternalVector.empty()) {
					item = std::move(mInternalVector.back());
					mInternalVector.pop_back();
				}
			}

			void swap(ConcurrentVector& other) {
				ScopedLock lockSelf(mMutex);
				ScopedLock lockOther(other.mMutex);

				std::swap(mInternalVector, other.mInternalVector);
			}

			const std::vector<T>& getInternalsUnsafe() const {
				return mInternalVector;
			}

			std::vector<T> copyInternals() const {
				ScopedLock lock(mMutex);
				return mInternalVector;
			}

			void remove_if(std::function<bool(const T& value)> condition) {
				ScopedLock lock(mMutex);
				
				auto it = std::remove_if(
					mInternalVector.begin(), 
					mInternalVector.end(), 
					condition
				);

				if (it != mInternalVector.end())
					mInternalVector.erase(it);
			}

		private:
			std::vector<T> mInternalVector;
			mutable Mutex mMutex;
			Condition mCondition;
		};
	}
}

#endif
