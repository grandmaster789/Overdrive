#ifndef OVERDRIVE_UTIL_UNIQUE_RESOURCE_H
#define OVERDRIVE_UTIL_UNIQUE_RESOURCE_H

#include <type_traits>

namespace overdrive {
	//based on isocpp paper 3949 (slightly modified)
	//TODO: once msvc actually supports noexcept, add it where appropriate to this class (almost everywhere)
	//TODO: figure out a way to do a make_unique_resource (I foresee problems with the constructor arguments)
	template <typename tResource, typename tDeleteFn>
	class UniqueResource {
	private:
		enum class eInvokationStrategy {
			INVOKE_ONCE,
			INVOKE_AGAIN
		};

	public:
		explicit UniqueResource(
			tResource&& resource,
			tDeleteFn&& deleter,
			bool useDeleter = true
		):
			mResource{ std::move(resource) },
			mDeleter{ std::move(deleter) },
			mInvokeDeleterOnDestruction{ useDeleter }
		{
		}

		UniqueResource(UniqueResource&& ur) :
			mResource{ std::move(ur.mResource) },
			mDeleter{ std::move(ur.mDeleter) },
			mInvokeDeleterOnDestruction{ ur.mInvokeDeleterOnDestruction }
		{
			ur.release();
		}

		UniqueResource& operator = (UniqueResource&& ur) {
			invokeDeleter(eInvokationStrategy::INVOKE_ONCE);

			mResource = std::move(ur.mResource);
			mDeleter = std::move(ur.mDeleter);
			mInvokeDeleterOnDestruction = ur.mInvokeDeleterOnDestruction;

			ur.release();

			return *this;
		}

		~UniqueResource() {
			invokeDeleter(eInvokationStrategy::INVOKE_ONCE);
		}

		const tResource& release() {
			mInvokeDeleterOnDestruction = false;
			return mResource;
		}

		void reset(tResource&& newResource) {
			invokeDeleter(eInvokationStrategy::INVOKE_AGAIN); // clean up, but expect to need to clean up again in the future
			mResource = std::move(newResource);
		}

		// resource access
		const tResource& get() const {
			return mResource;
		}

		operator const tResource& () const { // implicit cast to underlying resource
			return mResource;
		}

		tResource operator ->() const { // dereferencing will create a copy
			return mResource;
		}

		std::add_lvalue_reference_t<std::remove_pointer_t<tResource>> operator *() const {
			return *mResource; // This will only work if the resource is a pointer. Then again, if it isn't a pointer, why attempt to call the * operator?
		}

		// deleter access
		const tDeleteFn& getDeleter() const {
			return mDeleter;
		}

	private:
		void invokeDeleter(const eInvokationStrategy strategy) {
			if (mInvokeDeleterOnDestruction) {
				try {
					mDeleter(mResource);
				}
				catch (...) {
				}
			}
			
			mInvokeDeleterOnDestruction = (strategy == eInvokationStrategy::INVOKE_AGAIN);
		}

		UniqueResource(const UniqueResource&) = delete;	//no copy construct
		UniqueResource& operator = (const UniqueResource&) = delete; //no copy assign

	private:
		tResource mResource;
		tDeleteFn mDeleter;
		bool mInvokeDeleterOnDestruction;
	};

	template <typename tResource, typename tDeleterFn>
	UniqueResource<tResource, tDeleterFn> unique_resource(
		tResource&& resource,	// move the resource
		tDeleterFn deleter		// copy the deleter when appropriate
	) {
		return UniqueResource<tResource, tDeleterFn>{
			std::move(resource),
			std::move(deleter),
			true
		};
	}

	template <typename tResource, typename tDeleterFn>
	UniqueResource<tResource, tDeleterFn> unique_resource_checked(
		tResource resource,		// can't use move here because the resource might be invalid (in which case the original state should be maintained)
		tResource invalidState,	// the reference state for an invalid resource can't be moved either
		tDeleterFn deleter		// deleters should be copied anywaysS
	) {
		bool useDeleter = !(resource == invalidState);

		return UniqueResource<tResource, tDeleterFn>{
			std::move(resource),	//at this point we're working with a local copy, which *can* be moved
			std::move(deleter),
			useDeleter
		};
	}
}

#endif

