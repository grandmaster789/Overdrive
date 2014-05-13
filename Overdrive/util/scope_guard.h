#ifndef OVERDRIVE_UTIL_SCOPE_GUARD_H
#define OVERDRIVE_UTIL_SCOPE_GUARD_H

namespace overdrive {
	//based on isocpp paper 3949 (slightly modified)
	
	/*
		Typical use case -- schedule rollback unless all operations succeeded
	 */

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

#endif
