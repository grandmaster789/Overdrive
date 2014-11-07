#ifndef OVERDRIVE_CORE_TASKPROCESSOR_H
#define OVERDRIVE_CORE_TASKPROCESSOR_H

#include "core/task.h"
#include "util/concurrent_vector.h"
#include <cstdint>
#include <boost/thread.hpp>

namespace overdrive {
	namespace core {
		class TaskProcessor {
		private:
			typedef util::ConcurrentVector<detail::WrappedTask> TaskQueue;

		public:
			TaskProcessor(size_t numWorkers = 0); //use 0 for autodetect

			void addWork(Task t, bool repeating = false, bool background = false);
			
			// explicit aliases for various types of work
			void addRepeatingWork(Task t, bool background = false);
			void addBackgroundWork(Task t, bool repeating = false);
			void addRepeatingBackgroundWork(Task t);
			
			void start();
			void stop();

		private:
			void addWork(detail::WrappedTask t);	//places the task in the appropriate queue
			void execute(detail::WrappedTask t);

			TaskQueue mMainTasks;		//executed in this thread
			TaskQueue mBackgroundTasks;	//launched in a separate thread (use this for lengthy operations, such as I/O)

			bool mIsRunning;
			
			size_t mNumWorkers;
			boost::thread_group mBackgroundWorkers;
		};
	}
}

#endif
