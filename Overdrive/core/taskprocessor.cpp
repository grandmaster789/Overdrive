#include "taskprocessor.h"

namespace overdrive {
	namespace core {
		TaskProcessor& TaskProcessor::get() {
			static TaskProcessor instance;
			return instance;
		}

		TaskProcessor::TaskProcessor() :
			mIOService(),
			mWork(mIOService)
		{
		}

		void TaskProcessor::start() {
			mIOService.run();
		}

		void TaskProcessor::stop() {
			mIOService.stop();
		}
	}
}