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
				overdrive::core::Channel::broadcast(5);
				overdrive::core::Channel::broadcast(123);

				{
					FooHandler b;
					b.prefix = "b";
					overdrive::core::Channel::broadcast(1000);
				}

				{
					FooHandler c(a);
					c.prefix = "c";
					overdrive::core::Channel::broadcast(999);
				}

				{
					FooHandler d = createFoo("d");
					overdrive::core::Channel::broadcast(777);
				}

				overdrive::core::Channel::broadcast(12345);
			}
		}
	}
}