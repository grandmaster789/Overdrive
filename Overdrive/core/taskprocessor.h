#ifndef OVERDRIVE_CORE_TASKPROCESSOR_H
#define OVERDRIVE_CORE_TASKPROCESSOR_H

#include "task.h"
#include "util/concurrent_queue.h"

#include <cstdint>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

namespace overdrive {
	namespace core {
		class TaskProcessor:
			public boost::noncopyable
		{
		private:
			typedef util::ConcurrentQueue<detail::WrappedTask> TaskQueue;

		public:
			TaskProcessor(size_t numWorkers = 0); //use 0 for autodetect
			~TaskProcessor();

			void add(
				Task t, 
				bool repeating = false, 
				bool background = false
			);

			void add(detail::WrappedTask t); //places the task in the appropriate queue
			
			void start();
			void stop();

		private:
			void execute(detail::WrappedTask t);

			TaskQueue mMainTasks;		//executed in this thread
			TaskQueue mBackgroundTasks;	//launched in a separate thread (use this for lengthy operations, such as I/O)

			uint32_t mIsRunning : 1;
			
			size_t mNumWorkers;
			boost::thread_group mBackgroundWorkers;
		};
	}
}

#endif
