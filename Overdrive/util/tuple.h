#pragma once

#include <tuple>

namespace overdrive {
	namespace util {
		// Sean Parents' for_each_argument (in C++17 this could be simplified further using fold expressions)
		// [NOTE] the order of evaluation is not determined
		template <typename Fn, typename...Args>
		void for_each_argument(Fn fn, Args&&... args);

		// using a different technique
		template <typename Fn, typename...Ts>
		void for_each_in_tuple(Fn fn, const std::tuple<Ts...>& tuple);
	}
}


#include "tuple.inl"