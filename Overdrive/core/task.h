#ifndef OVERDRIVE_CORE_TASK_H
#define OVERDRIVE_CORE_TASK_H

#include "../boost.h"
#include "../std.h"


namespace overdrive {
	namespace core {
		namespace detail {
			/*
			 * Helper class for wrapping a 'task', mainly makes sure that exceptions are properly caught
			 * (exceptions and improper thread interruptions are logged as error messages)
			 *
			 * For passing arguments or composing a more complex function, use bind
			 */
			
			struct WrappedTask {
			public:
				explicit WrappedTask(std::function<void()> task);

				void operator()() const;

			private:
				std::function<void()> mUnwrappedTask;
			};

			WrappedTask make_wrapped(std::function<void()> task);
		}
	}
}

#endif
