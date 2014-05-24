#ifndef OVERDRIVE_UTIL_MOVEONCOPY_H
#define OVERDRIVE_UTIL_MOVEONCOPY_H

namespace overdrive {
	namespace util {
		// Wrapper for using move-only objects (such as threads, packaged_task etc.) appropriately with containers 
		// and such (like std::vector) that require CopyConstructible/CopyAssignable semantics
		template <typename tMovable>
		struct MoveOnCopy {
			explicit MoveOnCopy(tMovable&& object) :
				mObject(std::move(object))
			{
			}

			MoveOnCopy(const MoveOnCopy& moc) :
				mObject(std::move(moc.mObject))
			{
			}

			MoveOnCopy(MoveOnCopy&& moc) :
				mObject(std::move(moc.mObject))
			{
			}

			MoveOnCopy& operator = (const MoveOnCopy& moc) {
				mObject = std::move(moc.mObject);
				return *this;
			}

			MoveOnCopy& operator = (MoveOnCopy&& moc) {
				mObject = std::move(moc.mObject);
				return *this;
			}

			void operator()() {
				mObject();
			}

			tMovable& get() {
				return mObject;
			}

			tMovable release() {
				return std::move(mObject);
			}

			mutable tMovable mObject;
		};
	}
}

#endif
