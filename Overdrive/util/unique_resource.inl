#ifndef OVERDRIVE_UTIL_UNIQUE_RESOURCE_INL
#define OVERDRIVE_UTIL_UNIQUE_RESOURCE_INL

#include "util/unique_resource.h"

namespace overdrive {
	template <typename R, typename D>
	UniqueResource<R, D>::UniqueResource(
		R&& resource,
		D deleter,
		bool useDeleter
	):
		mResource{ std::move(resource) },
		mDeleter{ std::move(deleter) },
		mInvokeDeleterOnDestruction{ useDeleter }
	{
	}

	template <typename R, typename D>
	UniqueResource<R, D>::UniqueResource(UniqueResource&& ur) :
		mResource{ std::move(ur.mResource) },
		mDeleter{ std::move(ur.mDeleter) },
		mInvokeDeleterOnDestruction{ ur.mInvokeDeleterOnDestruction }
	{
		ur.release();
	}

	template <typename R, typename D>
	UniqueResource<R, D>& UniqueResource<R, D>::operator = (UniqueResource&& ur) {
		invokeDeleter(eInvokationStrategy::INVOKE_ONCE);

		mResource = std::move(ur.mResource);
		mDeleter = std::move(ur.mDeleter);
		mInvokeDeleterOnDestruction = ur.mInvokeDeleterOnDestruction;

		ur.release();

		return *this;
	}

	template <typename R, typename D>
	UniqueResource<R, D>& UniqueResource<R, D>::operator = (R&& resource) {
		invokeDeleter(eInvokationStrategy::INVOKE_ONCE);

		mResource = std::move(resource);

		mInvokeDeleterOnDestruction = true;

		return *this;
	}

	template <typename R, typename D>
	UniqueResource<R, D>::~UniqueResource() {
		invokeDeleter(eInvokationStrategy::INVOKE_ONCE);
	}

	template <typename R, typename D>
	const R& UniqueResource<R, D>::release() {
		mInvokeDeleterOnDestruction = false;
		return mResource;
	}

	template <typename R, typename D>
	void UniqueResource<R, D>::reset(R&& newResource) {
		invokeDeleter(eInvokationStrategy::INVOKE_AGAIN); // clean up, but expect to need to clean up again in the future
		mResource = std::move(newResource);
	}

	template <typename R, typename D>
	R& UniqueResource<R, D>::get() {
		return mResource;
	}

	template <typename R, typename D>
	const R& UniqueResource<R, D>::get() const {
		return mResource;
	}

	template <typename R, typename D>
	UniqueResource<R, D>::operator const R& () const { // implicit cast to underlying resource
		return mResource;
	}

	template <typename R, typename D>
	R UniqueResource<R, D>::operator ->() const { // dereferencing will create a copy
		return mResource;
	}

	template <typename R, typename D>
	std::add_lvalue_reference_t<std::remove_pointer_t<R>> UniqueResource<R, D>::operator *() const {
		return *mResource; // This will only work if the resource is a pointer. Then again, if it isn't a pointer, why attempt to call the * operator?
	}

	template <typename R, typename D>
	const D& UniqueResource<R, D>::getDeleter() const {
		return mDeleter;
	}

	template <typename R, typename D>
	void UniqueResource<R, D>::invokeDeleter(const eInvokationStrategy strategy) {
		if (mInvokeDeleterOnDestruction) {
			try {
				mDeleter(mResource);
			}
			catch (...) {
			}
		}

		mInvokeDeleterOnDestruction = (strategy == eInvokationStrategy::INVOKE_AGAIN);
	}

	template <typename R, typename D>
	UniqueResource<R, D> make_unique_resource(
		R&& resource,
		D deleter
	) {
		return UniqueResource<tResource, tDeleterFn>{
			std::move(resource),
			std::move(deleter),
			true
		};
	}

	template <typename R, typename D>
	UniqueResource<R, D> make_unique_resource_checked(
		R resource,		// can't use move here because the resource might be invalid (in which case the original state should be maintained)
		R invalidState,	// the reference state for an invalid resource can't be moved either
		D deleter		// deleters should be copied anywaysS
	) {
		bool useDeleter = !(resource == invalidState);

		return UniqueResource<R, D>{
			std::move(resource),	//at this point we're working with a local copy, which *can* be moved
			std::move(deleter),
			useDeleter
		};
	}
}

#endif

