#include "stdafx.h"
#include "clock.h"

namespace overdrive {
	namespace core {
		Clock::Clock() {
			mCurrentTimePoint = ClockType::now();
		}

		Clock::TimePoint Clock::currentFrame() const {
			return mCurrentTimePoint;
		}

		Clock::Duration Clock::deltaFrame() const {
			return mSinceLastFrame;
		}

		void Clock::update() {
			auto pt = ClockType::now();
			mSinceLastFrame = pt - mCurrentTimePoint;
			mCurrentTimePoint = pt;
		}

		std::ostream& operator << (std::ostream& os, const Clock& clock) {
			using namespace std::chrono;

			os 
				<< std::chrono::duration_cast<milliseconds>(clock.mSinceLastFrame).count() 
				<< "ms";

			return os;
		}
	}
}