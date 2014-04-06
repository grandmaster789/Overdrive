#include "taskqueue.h"

namespace overdrive {
	namespace util {
		TaskQueue::TaskQueue() :
			mStopping(false)
		{
		}

		void TaskQueue::stop() {
			Lock lock(mMutex);
			
			mStopping = true;
			lock.unlock();
			
			mCondition.notify_all();
		}

		void TaskQueue::push(const Task& task) {
			Lock lock(mMutex);

			if (mStopping)
				return;

			mTasks.push_back(task);

			lock.unlock();
			mCondition.notify_one();
		}

		TaskQueue::Task TaskQueue::pop() {
			Lock lock(mMutex);

			while (mTasks.empty()) {
				if (mStopping)
					return Task();

				mCondition.wait(lock);
			}

			Task result = mTasks.front();
			mTasks.pop_front();

			return result;
		}
	}
}