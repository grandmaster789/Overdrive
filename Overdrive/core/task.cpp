#include "task.h"
#include "logger.h"

namespace overdrive {
	namespace core {
		namespace detail {
			/*** WrappedTask ***/
			WrappedTask::WrappedTask() {}

			WrappedTask::WrappedTask(Task task, bool repeating, bool threadsafe, bool framesynced):
				mUnwrappedTask(std::move(task))
			{
				mIsRepeating = repeating;
				mIsThreadsafe = threadsafe;
				mIsFrameSynced = framesynced;
			}

			void WrappedTask::operator()() const {
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
				catch (...) {
					gLogSev(ERROR) << "Unknown exception";
				}
			}

			bool WrappedTask::isRepeating() const {
				return mIsRepeating;
			}

			bool WrappedTask::isThreadsafe() const {
				return mIsThreadsafe;
			}

			bool WrappedTask::isFrameSynced() const {
				return mIsFrameSynced;
			}
		}
	}

	core::detail::WrappedTask make_wrapped(
		Task task, 
		bool repeating, 
		bool threadsafe, 
		bool framesync
	) {
		return core::detail::WrappedTask(task, repeating, threadsafe, framesync);
	}
}
