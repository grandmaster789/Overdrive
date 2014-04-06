#include <iostream>

#include "std.h"
#include "boost.h"
#include "opengl.h"

#include "core/logger.h"
#include "core/taskprocessor.h"

#include "test/test.h"

int value = 0;

void test() {
	if (++value < 10) {
		gLog << "task " << value;
		overdrive::core::TaskProcessor::get().addTask(&test);
		throw std::runtime_error("exception test!");
	}
	else
		overdrive::core::TaskProcessor::get().stop();
}

int main() {
	overdrive::core::LogHelper initLog;

	overdrive::test::runAllTests();

	auto& proc = overdrive::core::TaskProcessor::get();

	proc.addTask(&test);
	
	proc.start(); //this will block until one of the tasks calls 'stop'
}