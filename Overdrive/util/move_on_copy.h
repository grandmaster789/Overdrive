#ifndef OVERDRIVE_UTIL_MOVEONCOPY_H
#define OVERDRIVE_UTIL_MOVEONCOPY_H

namespace overdrive {
	namespace util {
		// Wrapper for using move-only objects (such as threads, packaged_task etc.) appropriately with containers 
		// and such (like std::vector) that require CopyConstructible/CopyAssignable semantics
		template <typename tMovable>
		struct MoveOnCopy {
			explicit MoveOnCopy(tMovable&& object);
			MoveOnCopy(const MoveOnCopy& moc);
			MoveOnCopy(MoveOnCopy&& moc);

			MoveOnCopy& operator = (const MoveOnCopy& moc);
			MoveOnCopy& operator = (MoveOnCopy&& moc);

			void operator()();

			tMovable& get();
			tMovable release();

			mutable tMovable mObject;
		};
	}
}

#include "move_on_copy.inl"

#endif
