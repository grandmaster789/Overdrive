#include "task.h"
#include "logger.h"

namespace overdrive {
	namespace core {
		namespace detail {
			/*** WrappedTask ***/
			WrappedTask::WrappedTask() {}

			WrappedTask::WrappedTask(Task task, bool repeating, bool background):
				mUnwrappedTask(std::move(task))
			{
				mIsRepeating = repeating;
				mIsBackground = background;
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

			bool WrappedTask::isBackground() const {
				return mIsBackground;
			}
		}
	}

	core::detail::WrappedTask make_wrapped(
		Task task, 
		bool repeating, 
		bool background
	) {
		return core::detail::WrappedTask(task, repeating, background);
	}
}
