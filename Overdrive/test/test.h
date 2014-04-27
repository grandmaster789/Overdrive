#ifndef OVERDRIVE_TEST_TEST_H
#define OVERDRIVE_TEST_TEST_H

#include "test/core/core_test.h"
#include "test/util/util_test.h"

namespace overdrive {
	namespace test {
		inline void runAllTests() {
			runCoreTest();
			runUtilTest();
		}
	}
}

#endif
