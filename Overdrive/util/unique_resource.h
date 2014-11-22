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
			tDeleteFn deleter,
			bool useDeleter = true
		);
		UniqueResource(UniqueResource&& ur);
		UniqueResource& operator = (UniqueResource&& ur);
		UniqueResource& operator = (tResource&& resource);
		~UniqueResource();

		const tResource& release();
		void reset(tResource&& newResource);

		// resource access
		tResource& get();
		const tResource& get() const;
		operator const tResource& () const; // implicit cast to underlying resource
		tResource operator ->() const;		// dereferencing will create a copy

		std::add_lvalue_reference_t<std::remove_pointer_t<tResource>> operator *() const;
		const tDeleteFn& getDeleter() const; // deleter access

	private:
		void invokeDeleter(const eInvokationStrategy strategy);

		UniqueResource(const UniqueResource&) = delete;	//no copy construct
		UniqueResource& operator = (const UniqueResource&) = delete; //no copy assign

	private:
		tResource mResource;
		tDeleteFn mDeleter;
		bool mInvokeDeleterOnDestruction;
	};

	template <typename tResource, typename tDeleterFn>
	UniqueResource<tResource, tDeleterFn> make_unique_resource(
		tResource&& resource,	// move the resource
		tDeleterFn deleter		// copy the deleter when appropriate
	);

	template <typename tResource, typename tDeleterFn>
	UniqueResource<tResource, tDeleterFn> make_unique_resource_checked(
		tResource resource,		// can't use move here because the resource might be invalid (in which case the original state should be maintained)
		tResource invalidState,	// the reference state for an invalid resource can't be moved either
		tDeleterFn deleter		// deleters should be copied anywaysS
	);
}

#include "util/unique_resource.inl"

#endif

