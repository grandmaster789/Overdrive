#ifndef OVERDRIVE_UTIL_MAKE_ITERABLE_H
#define OVERDRIVE_UTIL_MAKE_ITERABLE_H

namespace overdrive {
	namespace util {
		// given a begin and end iterator (usually pointers), yield an object that allows for range-based for looping
		// caveats apply! make sure the specified pair of iterators is actually a range!
		namespace detail {
			template <typename T>
			class Iterable {
			public:
				Iterable() = default;
				Iterable(T begin, T end):
					mBegin(begin)
					mEnd(end)
				{
				}

				T begin() const {
					return *mBegin;
				}

				T end() const {
					return *mEnd;
				}

			private:
				T mBegin;
				T mEnd;
			};
		}

		template <typename T>
		inline detail::Iterable<T> make_iterable(
			T begin, 
			T end
		) {
			return detail::Iterable(begin, end);
		}

		// http://ldionne.github.io/mlp11-cppnow-2014/

	}
}

#endif
