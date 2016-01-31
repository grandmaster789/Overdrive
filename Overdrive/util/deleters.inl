#pragma once

#include "deleters.h"

namespace overdrive {
	namespace util {
		inline void FreeHelper::operator()(void* memory) {
			free(memory);
		}

		inline void FCloseHelper::operator()(FILE* handle) {
			fclose(handle);
		}

#if OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_WINDOWS
		inline void LocalFreeHelper::operator()(HLOCAL buffer) {
			::LocalFree(buffer);
		}
#endif
	}
}