#ifndef OVERDRIVE_TEST_CORE_H
#define OVERDRIVE_TEST_CORE_H

namespace overdrive {
	namespace test {
		namespace core {
			void testLogger();
			void testChannel();
		}

		inline void runCoreTest() {
			using namespace core;

			testLogger();
			testChannel();
		}
	}
}

#endif
