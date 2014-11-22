#ifndef OVERDRIVE_UTIL_SCOPE_GUARD_INL
#define OVERDRIVE_UTIL_SCOPE_GUARD_INL

#include "util/scope_guard.h"
#include <iostream>

namespace overdrive {
	namespace util {
		template <typename T>
		ScopeGuard<T>::ScopeGuard(T&& fn):
			mDeleter(std::move(fn)),
			mInvokeOnDestruction(true)
		{
		}

		template <typename T>
		ScopeGuard<T>::ScopeGuard(ScopeGuard&& sg):
			mDeleter{ std::move(sg.mDeleter) },
			mInvokeOnDestruction{ sg.mInvokeOnDestruction }
		{
			sg.mInvokeOnDestruction = false;
		}

		template <typename T>
		ScopeGuard<T>::~ScopeGuard() {
			if (mInvokeOnDestruction) {
				try {
					mDeleter();
				}
				catch (...) {
					// if the deleter function threw an exception, we should at least notify
					std::cerr << "ScopeGuard function threw an exception!\n";
				}
			}
		}

		template <typename T>
		void ScopeGuard<T>::release() {
			mInvokeOnDestruction = false;
		}
	}

	//for ease-of-use
	template <typename T>
	util::ScopeGuard<T> scope_guard(T&& deleter) {
		return util::ScopeGuard<T>(std::move(deleter));
	}
}

#define _OVERDRIVE_UTIL_TOKEN_CONCAT_INTERNAL(a, b) a ## b
#define _OVERDRIVE_UTIL_TOKEN_CONCAT(a, b) _OVERDRIVE_UTIL_TOKEN_CONCAT_INTERNAL(a, b)

#define _OVERDRIVE_UTIL_SCOPE_GUARD_INTERNAL_INSTANTIATE(lambdaName, guardName, ...) \
	auto lambda = [&] { __VA_ARGS__; }; \
	overdrive::util::ScopeGuard<decltype(lambda)> guardName(lambdaName);

#define _OVERDRIVE_UTIL_SCOPE_GUARD_INTERNAL_CREATE_NAMES(counter, ...) \
	_OVERDRIVE_UTIL_SCOPE_GUARD_INTERNAL_INSTANTIATE( \
		_OVERDRIVE_UTIL_TOKEN_CONCAT(_SCOPEGUARD_FN_, counter), \
		_OVERDRIVE_UTIL_TOKEN_CONCAT(_SCOPEGUARD_INSTANCE_, counter), \
		__VA_ARGS__ \
	)

#endif
