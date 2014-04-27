#include <iostream>
#include <exception>
#include "opengl.h"

#include "core/logger.h"
#include "core/taskprocessor.h"

#include "test/test.h"

int value = 0;
int bgValue = 0;

overdrive::core::TaskProcessor proc;

void test() {
	if (++value < 10) {
		gLog << "task " << value;
		throw std::runtime_error("exception test!");
	}
	else
		proc.stop();
}

void testBG() {
	gLog << "BG: " << ++bgValue;
}

int main() {
	overdrive::core::LogHelper initLog;

	//overdrive::test::runAllTests();
	proc.add(&test, true);
	proc.add(&testBG, true, true);

	proc.start();

//	boost::this_thread::sleep_for(boost::chrono::seconds(5));
}