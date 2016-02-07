#include "stdafx.h"
#include "spinlock.h"
#include "../preprocessor.h"

namespace overdrive {
	namespace util {
		void Spinlock::lock() {
			while (mIsLocked.test_and_set(std::memory_order_acquire)) {
				// [NOTE] this is a bit of mildly black magic ~ an empty loop has some 
				// undesirable effects such as branch misprediction, thread starvation
				// and/or unnecessarily high power consumption. On linux platforms the
				// 'usual' manner to solve this is to use 'cpu_relax()', while Windows
				// provides the 'YieldProcessor()' method. I'm developing on windows
				// and can't test the linux solution any time soon.

#if OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_WINDOWS
				YieldProcessor();
/*
#elif OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_LINUX
				cpu_relax();*/
#else
				; // as noted, this will work with some caveats
#endif
			}
		}

		void Spinlock::unlock() {
			mIsLocked.clear(std::memory_order_release);
		}
	}
}