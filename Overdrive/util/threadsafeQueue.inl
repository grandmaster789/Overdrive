#pragma once

#include "threadsafeQueue.h"

namespace overdrive {
	namespace util {
		template <typename T>
		void ThreadsafeQueue<T>::pushFront(const T& value) {
			ScopedLock lock(mMutex);
			mInternalQueue.push_front(value);
		}

		template <typename T>
		void ThreadsafeQueue<T>::pushBack(const T& value) {
			ScopedLock lock(mMutex);
			mInternalQueue.push_back(value);
		}

		template <typename T>
		bool ThreadsafeQueue<T>::tryPopFront(T& value) {
			ScopedLock lock(mMutex);
		
			if (!mInternalQueue.empty()) {
				value = mInternalQueue.front();
				mInternalQueue.pop_front();

				return true;
			}

			return false;
		}

		template <typename T>
		bool ThreadsafeQueue<T>::tryPopBack(T& value) {
			ScopedLock lock(mMutex);

			if (!mInternalQueue.empty()) {
				value = mInternalQueue.back();
				mInternalQueue.pop_back();
			}

			return false;
		}

		template <typename T>
		void ThreadsafeQueue<T>::clear() {
			ScopedLock lock(mMutex);
			mInternalQueue.clear();
		}

		template <typename T>
		size_t ThreadsafeQueue<T>::size() const {
			ScopedLock lock(mMutex);
			return mInternalQueue.size();
		}

		template <typename T>
		bool ThreadsafeQueue<T>::empty() const {
			ScopedLock lock(mMutex);
			return mInternalQueue.empty();
		}
	}
}
