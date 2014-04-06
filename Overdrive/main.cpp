#include <iostream>

#include "std.h"
#include "boost.h"
#include "opengl.h"

#include "core/logger.h"
#include "core/taskprocessor.h"

int value = 0;

void test() {
	if (++value < 10) {
		gLog << "task " << value;
		overdrive::core::TaskProcessor::get().addTask(&test);
	}
	else
		overdrive::core::TaskProcessor::get().stop();
}

int main() {
	overdrive::core::LogHelper initLog;

	auto& proc = overdrive::core::TaskProcessor::get();

	gLog << "ping";
	
	gLogSev(DEBUG) << "debug";
	gLogSev(INFO) << "info";
	gLogSev(WARNING) << "info";
	gLogSev(ERROR) << "info";
	gLogSev(FATAL) << "fatal";

	BOOST_LOG(overdrive::core::__gLogObject::get()) << "poing";

	proc.addTask(&test);
	proc.start();
}