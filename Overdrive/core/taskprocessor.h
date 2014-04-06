#ifndef OVERDRIVE_CORE_TASKPROCESSOR_H
#define OVERDRIVE_CORE_TASKPROCESSOR_H

#include "task.h"

namespace overdrive {
	namespace core {
		// Task processing singleton
		class TaskProcessor:
			private boost::noncopyable
		{
		public:
			static TaskProcessor& get();

			void start();
			void stop();

			template <typename tFunctor>
			inline void addTask(const tFunctor& fn) {
				mIOService.post(detail::make_wrapped(fn));
			}

		protected:
			TaskProcessor();

		private:
			boost::asio::io_service mIOService;
			boost::asio::io_service::work mWork;
		};
	}
}

#endif
