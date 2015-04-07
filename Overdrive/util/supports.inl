#pragma once

#include "supports.h"

namespace overdrive {
	namespace util {
		namespace {
			template <
				typename Fn,
				typename... tArgs,
				typename = decltype(std::declval<Fn>()(std::declval<tArgs>()...))
			> std::true_type supports_test(const Fn&, const tArgs&...);
			std::false_type supports_test(...);
		}

		// partial specialization for function calls
		template <typename Fn, typename...tArgs>
		struct Supports<Fn(tArgs...)> :
			decltype(
				supports_test(
					std::declval<Fn>(),
					std::declval<tArgs>()...
				)
			)
		{
		};
	}
}