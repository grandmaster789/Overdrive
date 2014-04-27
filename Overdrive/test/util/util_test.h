#ifndef OVERDRIVE_TEST_UTIL_H
#define OVERDRIVE_TEST_UTIL_H

namespace overdrive {
	namespace test {
		namespace util {
			void testNamed();
		}

		inline void runUtilTest() {
			using namespace util;

			testNamed();
		}
	}
}

#endif
