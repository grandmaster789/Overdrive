#pragma once

#include "tuple.h"

namespace overdrive {
	namespace util {
		template <typename Fn, typename...Args>
		void for_each_argument(Fn fn, Args&&... args) {
			[](...) {} (
				(fn(std::forward<Args>(args)), 0),
				...
			);
		}

		namespace detail {
			template <typename Fn, typename...Ts>
			void apply_in_tuple(
				Fn fn,
				const std::tuple<Ts...>& tuple,
				std::integral_constant<size_t, sizeof...(Ts)>
			) {
			}

			template <typename Fn, typename...Ts, size_t Idx>
			void apply_in_tuple(
				Fn fn,
				const std::tuple<Ts...>& tuple,
				std::integral_constant<size_t, Idx>
			) {
				fn(std::get<Idx>(tuple));

				apply_in_tuple(
					fn, 
					tuple, 
					std::integral_constant<size_t, Idx + 1>()
				);
			}
		}

		template <typename Fn, typename...Ts>
		void for_each_in_tuple(Fn fn, const std::tuple<Ts...>& tuple) {
			detail::apply_in_tuple(
				fn, 
				tuple, 
				std::integral_constant<size_t, 0>()
			);
		}
	}
}