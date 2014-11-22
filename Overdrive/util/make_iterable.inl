#ifndef OVERDRIVE_UTIL_MAKE_ITERABLE_INL
#define OVERDRIVE_UTIL_MAKE_ITERABLE_INL

#include "util/make_iterable.h"

namespace overdrive {
	namespace util {
		namespace detail {
			template <typename T>
			Iterable<T>::Iterable(T begin, T end) :
				mBegin(begin)
				mEnd(end)
			{
			}

			template <typename T>
			T Iterable::begin() const {
				return *mBegin;
			}

			template <typename T>
			T Iterable::end() const {
				return *mEnd;
			}
		}

		template <typename T>
		detail::Iterable<T> make_iterable(
			T begin,
			T end
		) {
			return detail::Iterable(begin, end);
		}

	}
}

#endif
