#include "timer.h"

namespace overdrive {
	namespace util {
		Timer::Timer() :
			mCurrentTime{ 0.0f },
			mDeltaTime{ 0.0f },
			mIsPaused{ true }
		{
		}

		float Timer::getCurrentTime() const {
			return mCurrentTime;
		}

		float Timer::getDeltaTime() const {
			return mDeltaTime;
		}

		bool Timer::isPaused() const {
			return mIsPaused;

		}

		void Timer::setPaused(bool flag) {
			mIsPaused = flag;
		}

		void Timer::update(float delta) {
			mDeltaTime = delta;
			mCurrentTime += delta;
		}
	}
}