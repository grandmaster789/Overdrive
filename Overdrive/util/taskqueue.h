#ifndef OVERDRIVE_UTIL_TASKQUEUE_H
#define OVERDRIVE_UTIL_TASKQUEUE_H

#include "../boost.h"
#include "../std.h"

namespace overdrive {
	namespace util {
		// FIFO threadsafe queue for void functions
		// (note 1. the contained type could pretty much be anything that provides a default constructor)
		// (note 2. while this refers to Tasks, it is not actually related to core::detail::WrappedTask)
		class TaskQueue {
		public:
			typedef boost::function<void()> Task;

			TaskQueue();

			void stop(); // this must explicitly be called when shutting down

			void push(const Task& task);
			Task pop(); // this will block while no tasks are available

		private:
			typedef std::deque<Task> InternalQueue;
			typedef boost::mutex Mutex;
			typedef boost::condition_variable Condition;
			typedef boost::unique_lock<Mutex> Lock;

			InternalQueue mTasks;
			Mutex mMutex;
			Condition mCondition;
			bool mStopping;
		};
	}
}

#endif