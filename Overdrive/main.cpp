#include <iostream>
#include <exception>
#include "opengl.h"

#include "core/channel.h"
#include "core/logger.h"
#include "core/taskprocessor.h"
#include "core/engine.h"
#include "core/system.h"

#include "util/named.h"

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
		mEngine->updateSystem(this, true, false);

		return true;
	}

	void update() override {
		++mCounter;

		gLog << "Count: " << mCounter;

		if (mCounter > 10)
			mEngine->stop();
	}

	void shutdown() override {
		System::shutdown();
		gLog << "Final counter: " << mCounter;
	}

private:
	int mCounter = 0;
};

int main() {
	//overdrive::test::runAllTests();

	overdrive::core::Engine engine;

	engine.add(new TestSystem());
	engine.run();
}