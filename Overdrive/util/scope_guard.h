#ifndef OVERDRIVE_UTIL_SCOPE_GUARD_H
#define OVERDRIVE_UTIL_SCOPE_GUARD_H

#include <iostream>

namespace overdrive {
	//based on isocpp paper 3949 (slightly modified)
	
	namespace util {
		template <typename tDeleteFn>
		struct ScopeGuard {
		public:
			explicit ScopeGuard(tDeleteFn&& fn) :
				mDeleter{ std::move(fn) },
				mInvokeOnDestruction{ true }
			{
			}

			ScopeGuard(ScopeGuard&& sg) :
				mDeleter{ std::move(sg.mDeleter) },
				mInvokeOnDestruction{ sg.mInvokeOnDestruction }
			{
				sg.mInvokeOnDestruction = false;
			}

			~ScopeGuard() {
				if (mInvokeOnDestruction) {
					try {
						mDeleter();
					}
					catch (...) {
						// if the deleter function threw an exception, we should at least notify us
						std::cerr << "ScopeGuard function threw an exception!\n";
					}
				}
			}

			void release() {
				mInvokeOnDestruction = false;
			}

		private:
			ScopeGuard(const ScopeGuard&) = delete; //no copy construct
			ScopeGuard& operator = (const ScopeGuard&) = delete; //no copy assign
			ScopeGuard& operator = (ScopeGuard&&) = delete; //no move-assign

		private:
			tDeleteFn mDeleter;
			bool mInvokeOnDestruction;
		};
	}

	//for ease-of-use
	template <typename tDeleteFn>
	util::ScopeGuard<tDeleteFn> scope_guard(tDeleteFn&& deleter) {
		return util::ScopeGuard<tDeleteFn>(std::move(deleter));
	}

	//usage: auto guard = scope_guard([] { std::cout << "done\n"; });
}

// for even easier use (auto-generate unique guard names at compile time):
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

#define SCOPE_GUARD(...) _OVERDRIVE_UTIL_SCOPE_GUARD_INTERNAL_CREATE_NAMES(__COUNTER__, __VA_ARGS__)

// usage: SCOPE_GUARD([] { std::cout << "performed at scope exit\n"; })
//	the local guard name is automatically generated, so no 'auto guard = ' is needed anymore
//	the 'downside' is that it becomes impossible to move/release the guard -- if you want to do that, you should make it named anyway

#endif
