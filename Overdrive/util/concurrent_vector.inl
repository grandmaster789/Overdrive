#ifndef OVERDRIVE_UTIL_CONCURRENT_VECTOR_INL
#define OVERDRIVE_UTIL_CONCURRENT_VECTOR_INL

#include "util/concurrent_vector.h"

namespace overdrive {
	namespace util {
		template <typename T>
		void ConcurrentVector<T>::push_back(const T& item) {
			ScopedLock lock(mMutex);
			mInternalVector.push_back(item);
			mCondition.notify_all();
		}

		template <typename T>
		void ConcurrentVector<T>::push_back(T&& item) {
			ScopedLock lock(mMutex);
			mInternalVector.push_back(std::move(item));
			mCondition.notify_all();
		}

		template <typename T>
		void ConcurrentVector<T>::pop_back(T& item) {
			UniqueLock lock(mMutex);

			mCondition.wait(lock, [&] { return !mInternalVector.empty(); });

			if (!mInternalVector.empty()) {
				item = std::move(mInternalVector.back());
				mInternalVector.pop_back();
			}
		}

		template <typename T>
		void ConcurrentVector<T>::swap(ConcurrentVector& other) {
			ScopedLock lockSelf(mMutex);
			ScopedLock lockOther(other.mMutex);

			std::swap(mInternalVector, other.mInternalVector);
		}

		template <typename T>
		const std::vector<T>& ConcurrentVector<T>::getInternalsUnsafe() const {
			return mInternalVector;
		}

		template <typename T>
		std::vector<T> ConcurrentVector<T>::copyInternals() const {
			ScopedLock lock(mMutex);
			return mInternalVector;
		}

		template <typename T>
		void ConcurrentVector<T>::remove_if(std::function<bool(const T& value)> condition) {
			ScopedLock lock(mMutex);
			
			auto it = std::remove_if(
				mInternalVector.begin(),
				mInternalVector.end(),
				condition
			);

			if (it != mInternalVector.end())
				mInternalVector.erase(it);
		}
	}
}

#endif
