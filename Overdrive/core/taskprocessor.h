#ifndef OVERDRIVE_CORE_TASKPROCESSOR_H
#define OVERDRIVE_CORE_TASKPROCESSOR_H

#include "task.h"

namespace overdrive {
	// typedefs for timing tasks
	typedef boost::asio::deadline_timer::time_type Time;
	typedef boost::asio::deadline_timer::duration_type Duration;
	

	namespace core {
		// Task processing singleton
		class TaskProcessor:
			private boost::noncopyable
		{
		public:
			static TaskProcessor& get();

			void start();
			void stop();

			void addTask(std::function<void()> fn);

		protected:
			TaskProcessor();

		private:
			boost::asio::io_service mIOService;
			boost::asio::io_service::work mWork;
		};
	}
}

#endif
