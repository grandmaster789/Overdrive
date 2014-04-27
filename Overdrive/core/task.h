#ifndef OVERDRIVE_CORE_TASK_H
#define OVERDRIVE_CORE_TASK_H

#include <cstdint>
#include <functional>

namespace overdrive {
	// The main Task typedef
	typedef std::function<void()> Task;

	namespace core {
		namespace detail {
			/*
			 * Helper class for wrapping a 'task', mainly makes sure that exceptions are properly caught
			 * (exceptions and improper thread interruptions are logged as error messages)
			 *
			 * Also adds an interface that tells the processor how to schedule the tasks' execution
			 *
			 * For passing arguments or composing a more complex function, use std::bind / boost::bind
			 */
			
			struct WrappedTask {
			public:
				explicit WrappedTask();
				explicit WrappedTask(Task t, bool repeating, bool threadsafe, bool framesynced);

				void operator()() const;
				
				bool isRepeating() const;
				bool isThreadsafe() const;
				bool isFrameSynced() const;

				friend class TaskProcessor;

			private:
				Task mUnwrappedTask;
				uint32_t mIsRepeating : 1;
				uint32_t mIsThreadsafe : 1;
				uint32_t mIsFrameSynced : 1;
			};
		}
	}

	//in the end, this became a trivial function
	core::detail::WrappedTask make_wrapped(
		Task task, 
		bool repeating = false, 
		bool threadsafe = false, 
		bool framesync = false
	);
}

#endif
