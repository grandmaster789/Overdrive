#ifndef OVERDRIVE_CORE_TASK_H
#define OVERDRIVE_CORE_TASK_H

#include "../boost.h"
#include "../std.h"
#include "logger.h"

namespace overdrive {
	namespace core {
		namespace detail {
			/*
			* Helper class for wrapping a 'task', mainly makes sure that exceptions are properly caught
			*
			* The actual task should implement a void operator() -- a basic functor signature
			* For passing arguments or composing a more complex function, use bind
			*/

			template <typename T>
			struct WrappedTask {
			public:
				explicit WrappedTask(const T& unwrappedTask) :
					mUnwrappedTask(unwrappedTask)
				{
				}

				void operator()() const {
					// reset interruption
					try {
						boost::this_thread::interruption_point();
					}
					catch (const boost::thread_interrupted&) {}

					try {
						// execute the task
						mUnwrappedTask();
					}
					catch (const std::exception& ex) {
						gLogSev(ERROR) << "Exception: " << ex.what();
					}
					catch (const boost::thread_interrupted&) {
						gLogSev(ERROR) << "Thread interrupted";
					}
					catch (...) {
						gLogSev(ERROR) << "Unknown exception";
					}
				}

			private:
				T mUnwrappedTask;
			};

			template <typename T>
			WrappedTask<T> make_wrapped(const T& task) {
				return detail::WrappedTask<T>(task);
			}
		}
	}
}

#endif
