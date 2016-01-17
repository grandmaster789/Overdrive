#pragma once

#include <chrono>
#include <ostream>

namespace overdrive {
	namespace core {
		/*
			[TODO] virtual game time?
		*/
		class Clock {
		private:
			friend class Engine;

			typedef std::chrono::high_resolution_clock ClockType;
			typedef ClockType::time_point TimePoint;
			typedef ClockType::duration Duration;

		public:
			Clock();

			TimePoint currentFrame() const; // yields the time at the start of the current frame
			std::chrono::milliseconds deltaFrame() const; // yields the duration since the last frame
			
			friend std::ostream& operator << (std::ostream& os, const Clock& clock);

		private:
			void update();

			TimePoint mCurrentTimePoint;
			Duration mSinceLastFrame;
		};
	}
}