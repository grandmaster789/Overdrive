#include <iostream>
#include <exception>
#include "opengl.h"

#include "core/channel.h"
#include "core/logger.h"
#include "core/taskprocessor.h"

#include "test/test.h"

int value = 0;
int bgValue = 0;

int main() {
	overdrive::core::LogHelper initLog;

	overdrive::test::runAllTests();
}