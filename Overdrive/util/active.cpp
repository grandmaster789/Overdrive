#include "stdafx.h"
#include "active.h"

namespace overdrive {
	namespace util {
		Active::Active() :
			mIsDone(false)
		{
		}

		Active::~Active() {
			send([this] { mIsDone = true; });

			mThread.join();
		}

		std::unique_ptr<Active> Active::create() {
			using std::unique_ptr;
			using boost::thread;

			unique_ptr<Active> result(new Active); // this constructor is private, so make_unique would require some hassle to get to work

			result->mThread = thread(&Active::run, result.get());

			return result;
		}

		void Active::send(Callback message) {
			mMessageQueue.push_back(
				std::move(message)
			);
		}

		void Active::run() {
			while (!mIsDone) {
				Callback fn;

				mMessageQueue.wait_pull_front(fn);

				fn();
			}
		}
	}
}