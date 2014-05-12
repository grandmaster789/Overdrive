#include "core/channel.h"
#include <iostream>
#include <string>

namespace overdrive {
	namespace test {
		namespace core {
			namespace {
				struct FooHandler :
					public overdrive::core::MessageHandler<int>
				{
					std::string prefix;

					void operator()(const int& value) {
						std::cout << prefix << " Received: " << value << "\n";
					}
				};

				FooHandler createFoo(std::string prefix) {
					FooHandler result;
					result.prefix = prefix;
					return result;
				}

			}

			void testChannel() {
				FooHandler a;
				a.prefix = "a";
				overdrive::core::Channel chan;

				chan.broadcast(5);
				chan.broadcast(123);

				{
					FooHandler b;
					b.prefix = "b";
					chan.broadcast(1000);
				}

				{
					FooHandler c(a);
					c.prefix = "c";
					chan.broadcast(999);
				}

				{
					FooHandler d = createFoo("d");
					chan.broadcast(777);
				}

				chan.broadcast(12345);
			}
		}
	}
}