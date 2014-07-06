#include "core/taskprocessor.h"
#include "core/logger.h"

namespace overdrive {
	namespace core {
		TaskProcessor::TaskProcessor(size_t numWorkers) :
			mNumWorkers(numWorkers),
			mIsRunning(false)
		{
			if (mNumWorkers == 0)
				mNumWorkers = boost::thread::hardware_concurrency() - 1; // look at the available number of hardware threads, but leave one for the main thread
			
			if (mNumWorkers < 1)
				mNumWorkers = 1; // keep at least 1 background thread
		}

		void TaskProcessor::addWork(Task t, bool repeating, bool background) {
			addWork(make_wrapped(std::move(t), repeating, background));
		}
		
		void TaskProcessor::addRepeatingWork(Task t, bool background) {
			addWork(std::move(t), true, background);
		}

		void TaskProcessor::addBackgroundWork(Task t, bool repeating) {
			addWork(std::move(t), repeating, true);
		}

		void TaskProcessor::addRepeatingBackgroundWork(Task t) {
			addWork(std::move(t), true, true);
		}

		void TaskProcessor::start() {
			if (mIsRunning)
				return; // already started

			mIsRunning = true;

			gLog << "Starting " << mNumWorkers << " background threads";

			// start the workers
			for (size_t i = 0; i < mNumWorkers; ++i) {
				mBackgroundWorkers.create_thread([&] {
					while (mIsRunning) {
						detail::WrappedTask t;

						mBackgroundTasks.pop_back(t);

						if (mIsRunning)
							execute(t);
					}
				});
			}

			// start the main executor
			while (mIsRunning) {
				TaskQueue localQueue;
				mMainTasks.swap(localQueue);

				const auto& taskList = localQueue.getInternalsUnsafe();
				for (const auto& task: taskList)
					execute(task);
			}
		}

		void TaskProcessor::stop() {
			mIsRunning = false;

			for (size_t i = 0; i < mNumWorkers; ++i)
				addBackgroundWork([&] { mIsRunning = false; });

			mBackgroundWorkers.join_all();
		}

		void TaskProcessor::addWork(detail::WrappedTask t) {
			if (t.isBackground())
				mBackgroundTasks.push_back(std::move(t));
			else
				mMainTasks.push_back(std::move(t));
		}

		void TaskProcessor::execute(detail::WrappedTask t) {
			t();

			if (t.isRepeating())
				addWork(std::move(t));
		}
	}
}
