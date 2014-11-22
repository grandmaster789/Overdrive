#ifndef OVERDRIVE_UTIL_MOVEONCOPY_INL
#define OVERDRIVE_UTIL_MOVEONCOPY_INL

namespace overdrive {
	namespace util {
		// Wrapper for using move-only objects (such as threads, packaged_task etc.) appropriately with containers 
		// and such (like std::vector) that require CopyConstructible/CopyAssignable semantics
		template <typename T>
		MoveOnCopy<T>::MoveOnCopy(T&& object) :
			mObject(std::move(object))
		{
		}

		template <typename T>
		MoveOnCopy<T>::MoveOnCopy(const MoveOnCopy& moc) :
			mObject(std::move(moc.mObject))
		{
		}

		template <typename T>
		MoveOnCopy<T>::MoveOnCopy(MoveOnCopy&& moc) :
			mObject(std::move(moc.mObject))
		{
		}

		template <typename T>
		MoveOnCopy<T>& MoveOnCopy<T>::operator = (const MoveOnCopy& moc) {
			mObject = std::move(moc.mObject);
			return *this;
		}

		template <typename T>
		MoveOnCopy<T>& MoveOnCopy<T>::operator = (MoveOnCopy&& moc) {
			mObject = std::move(moc.mObject);
			return *this;
		}

		template <typename T>
		void MoveOnCopy<T>::operator()() {
			mObject();
		}

		template <typename T>
		T& MoveOnCopy<T>::get() {
			return mObject;
		}

		template <typename T>
		T MoveOnCopy<T>::release() {
			return std::move(mObject);
		}
	}
}

#endif
