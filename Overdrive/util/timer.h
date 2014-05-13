#ifndef OVERDRIVE_UTIL_TIMER_H
#define OVERDRIVE_UTIL_TIMER_H

namespace overdrive {
	namespace util {
		// Helper class to store the current time and the delta since the last frame update
		// [Note]	This does not actually query the system clock; proper usage can only occur by
		//			manually updating the internal values
		class Timer {
		public:
			Timer();

			float getCurrentTime() const;
			float getDeltaTime() const;
			
			bool isPaused() const;
			void setPaused(bool flag);
			
			void update(float delta);

		protected:
			float mCurrentTime;
			float mDeltaTime;
			bool mIsPaused;
		};
	}
}

#endif
