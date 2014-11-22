#ifndef OVERDRIVE_UTIL_MAKE_ITERABLE_H
#define OVERDRIVE_UTIL_MAKE_ITERABLE_H

namespace overdrive {
	namespace util {
		// given a begin and end iterator (usually pointers), yield an object that allows for range-based for looping
		// caveats apply! make sure the specified pair of iterators is actually a range!
		// will work for pointer pairs and integer pairs (pretty much nothing else though)
		// [TODO] add enable_if's
		namespace detail {
			template <typename T>
			class Iterable {
			public:
				Iterable() = default;
				Iterable(T begin, T end);

				T begin() const;
				T end() const;

			private:
				T mBegin;
				T mEnd;
			};
		}

		template <typename T>
		inline detail::Iterable<T> make_iterable(T begin, T end);
	}
}

#include "make_iterable.inl"

#endif
