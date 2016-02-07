#pragma once

#include <atomic>

namespace overdrive {
	namespace util {
		class Spinlock {
		public:
			Spinlock(const Spinlock&) = delete;
			Spinlock& operator = (const Spinlock&) = delete;

			void lock();
			void unlock();

		private:
			std::atomic_flag mIsLocked = ATOMIC_FLAG_INIT;
		};
	}
}
