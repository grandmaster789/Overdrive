#include "core/task.h"
#include "core/logger.h"
#include <boost/thread.hpp>

namespace overdrive {
	namespace core {
		namespace detail {
			/*** WrappedTask ***/
			WrappedTask::WrappedTask() {}

			WrappedTask::WrappedTask(Task task, bool repeating, bool background):
				mUnwrappedTask{ std::move(task) }
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
					gLog.error() << "Exception: " << ex.what();
				}
				catch (...) {
					gLog.error() << "Unknown exception";
				}
			}

			bool WrappedTask::isRepeating() const {
				return mIsRepeating;
			}

			bool WrappedTask::isBackground() const {
				return mIsBackground;
			}

			void WrappedTask::setRepeating(bool enabled) {
				mIsRepeating = enabled;
			}

			void WrappedTask::setBackground(bool enabled) {
				mIsBackground = enabled;
			}
		}
	}

	core::detail::WrappedTask make_wrapped(
		Task task, 
		bool repeating, 
		bool background
	) {
		return core::detail::WrappedTask{ task, repeating, background };
	}
}
