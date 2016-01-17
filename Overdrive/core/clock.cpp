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

		std::chrono::milliseconds Clock::deltaFrame() const {
			return std::chrono::duration_cast<std::chrono::milliseconds>(mSinceLastFrame);
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