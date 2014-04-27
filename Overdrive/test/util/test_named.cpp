#include "util/named.h"
#include <cassert>

namespace overdrive {
	namespace test {
		namespace util {
			struct NamedTestFoo :
				public ::overdrive::util::Named
			{
				NamedTestFoo(std::string name) :
					Named(name)
				{}

				void toBar() {
					setName("Bar"); // protected function, only accessible by the class itself
				}
			};

			void testNamed() {
				NamedTestFoo foo("Foo");
				NamedTestFoo bar("Bar");

				assert(foo.getName() == "Foo");
				assert(bar.getName() == "Bar");

				foo.toBar();

				assert(foo.getName() == "Bar");
			}
		}
	}
}