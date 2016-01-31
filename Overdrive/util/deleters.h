#pragma once

#include "../preprocessor.h"

#include <cstdio>
#include <cstdlib>

namespace overdrive {
	namespace util {
		//
		// Small collection of Deleter objects that can be used with a unique_ptr for common 'close' idioms
		//

		struct FreeHelper {
			void operator ()(void* memory); // when the memory was allocated using malloc
		};

		struct FCloseHelper {
			void operator()(FILE* filehandle); // when the handle was created using fopen
		};

#if OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_WINDOWS
		struct LocalFreeHelper {
			void operator()(HLOCAL buffer); // for when a buffer is allocated using LocalAlloc
		};
#endif
	}
}

#include "deleters.inl"