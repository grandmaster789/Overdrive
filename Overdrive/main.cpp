#include <iostream>

#include "std.h"
#include "boost.h"
#include "opengl.h"

#include "core/logger.h"



int main() {
	overdrive::core::LogHelper initLog;

	gLog << "ping";
	
	gLogSev(DEBUG) << "debug";
	gLogSev(INFO) << "info";
	gLogSev(WARNING) << "info";
	gLogSev(ERROR) << "info";
	gLogSev(FATAL) << "fatal";

	BOOST_LOG(overdrive::core::__gLogObject::get()) << "poing";
}