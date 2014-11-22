#ifndef OVERDRIVE_UTIL_SCOPE_GUARD_H
#define OVERDRIVE_UTIL_SCOPE_GUARD_H

namespace overdrive {
	//based on isocpp paper 3949 (slightly modified)
	
	namespace util {
		template <typename tDeleteFn>
		struct ScopeGuard {
		public:
			explicit ScopeGuard(tDeleteFn&& fn);
			ScopeGuard(ScopeGuard&& sg);
			~ScopeGuard();

			void release();

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
	//usage: auto guard = scope_guard([] { std::cout << "done\n"; });
	template <typename tDeleteFn>
	util::ScopeGuard<tDeleteFn> scope_guard(tDeleteFn&& deleter);
}

#include "util/scope_guard.inl"

// for even easier use (auto-generate unique guard names at compile time):

#define SCOPE_GUARD(...) _OVERDRIVE_UTIL_SCOPE_GUARD_INTERNAL_CREATE_NAMES(__COUNTER__, __VA_ARGS__)

// usage: SCOPE_GUARD([] { std::cout << "performed at scope exit\n"; })
//	the local guard name is automatically generated, so no 'auto guard = ' is needed anymore
//	the 'downside' is that it becomes impossible to move/release the guard -- if you want to do that, you should make it named anyway

#endif
