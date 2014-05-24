#include <iostream>
#include <exception>
#include "opengl.h"

#include <future>

#include "core/channel.h"
#include "core/logger.h"
#include "core/taskprocessor.h"
#include "core/engine.h"
#include "core/system.h"

#include "util/named.h"
#include "util/concurrent.h"

#include "test/test.h"

class TestSystem:
	public overdrive::core::System
{
public:
	TestSystem() :
		System("Test")
	{
	}

	bool initialize() override {
		System::initialize();
		mEngine->updateSystem(this, true, true);

		return true;
	}

	void update() override {
		++mCounter;

		gLog << "Count: " << mCounter;

		if (mCounter >= 10)
			mEngine->stop();
	}

	void shutdown() override {
		System::shutdown();
		gLog << "Final counter: " << mCounter;
	}

private:
	int mCounter = 0;
};

struct TestConcurrent {
	void foo() {
		gLog << "FOO";
	}

	int gimme() {
		return 5;
	}
};

void baz() {
	gLog << "BAZ";
}

int main() {
	//overdrive::test::runAllTests();

	overdrive::util::Concurrent<TestConcurrent> bar;
	//bar.lambda([](TestConcurrent& obj) { obj.foo(); });
	//std::future<int> i = bar.lambda([](TestConcurrent& obj) { return obj.gimme(); });
	//bar.call(&TestConcurrent::foo);
	auto i = bar.call(&TestConcurrent::gimme);

	gLog << "Gimme: " << i.get();

	std::packaged_task<void()> task(&baz);
	auto fut = task.get_future();
	task();

	overdrive::core::Engine engine;

	engine.add(new TestSystem());
	engine.run();
}