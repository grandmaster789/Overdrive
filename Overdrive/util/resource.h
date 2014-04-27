#ifndef OVERDRIVE_UTIL_RESOURCE_H
#define OVERDRIVE_UTIL_RESOURCE_H

#include <memory>
#include <functional>
#include <utility>

namespace overdrive {
	namespace util {
		template <
			typename tCreateFn,
			typename tDestroyFn,
			typename... tCreateArgs
		>
		auto make_resource(
			tCreateFn createFn,
			tDestroyFn destroyFn,
			tCreateArgs&&... args
		)
			-> std::unique_ptr<typename std::decay<decltype((*createFn)(std::forward<tCreateArgs>(args)...))>::type> //ugh... c++14 plx :(
		{
			auto resource = createFn(std::forward<tCreateArgs>(args)...);

			if (!resource)
				throw std::runtime_error{ "Unable to create resource" };

			typedef typename std::decay<decltype(*resource)> ResourceType;

			return std::unique_ptr<ResourceType, void(*)(ResourceType*)>(resource, destroyFn);
		}
	}
}

#endif

