#include "task.h"
#include "logger.h"

namespace overdrive {
	namespace core {
		namespace detail {
			WrappedTask::WrappedTask(std::function<void()> task) :
				mUnwrappedTask(task)
			{
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
				catch (const boost::thread_interrupted&) {
					gLogSev(ERROR) << "Thread interrupted";
				}
				catch (...) {
					gLogSev(ERROR) << "Unknown exception";
				}
			}

			WrappedTask make_wrapped(std::function<void()> task) {
				return WrappedTask(task);
			}
		}
	}
}
