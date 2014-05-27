#ifndef OVERDRIVE_CONCURRENT_H
#define OVERDRIVE_CONCURRENT_H

#include <memory>
#include <functional>
#include <type_traits>
#include <stdexcept>
#include <thread>
#include <future>

#include "util/move_on_copy.h"
#include "util/concurrent_queue.h"

// Lightweight active object wrapper, mostly inspired by Herb Sutter's presentation
//		 http://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Herb-Sutter-Concurrency-and-Parallelism
namespace overdrive {
	namespace util {
		namespace detail {
			typedef std::function<void()> Task;

			// helper for non-void promises
			template <typename Fut, typename F, typename T>
			void set_promise_value(std::promise<Fut>& promise, F& f, T& t) {
				promise.set_value(f(t));
			}

			// helper for void promises
			template <typename F, typename T>
			void set_promise_value(std::promise<void>& promise, F& f, T& t) {
				f(t);
				promise.set_value();
			}
		}

		template <typename T>
		class Concurrent {
		public:
			Concurrent(const Concurrent&) = delete;
			Concurrent& operator = (const Concurrent&) = delete;

			template <typename... Args>
			Concurrent(Args&&... args) :
				Concurrent(std::make_unique<T>(std::forward<Args>(args)...))
			{
			}

			Concurrent(std::unique_ptr<T> object) :
				mWrappedObject(std::move(object)),
				mThread([=] {
					detail::Task task;

					while (mWrappedObject && !mDone) {
						mTaskQueue.pop(task);
						task();
					}
				})
			{
			}

			virtual ~Concurrent() {
				mTaskQueue.push([=] { mDone = true; });
				
				if (mThread.joinable())
					mThread.join();
			}

			bool invalid() const {
				return !mWrappedObject;
			}

			template <typename tFunctionPtr, typename... Args>
			auto call(tFunctionPtr fn, Args&&... args) const
				->std::future<typename std::result_of< decltype(fn)(T*, Args...)>::type>
			{
				typedef typename std::result_of<decltype(fn)(T*, Args...)>::type Result;
				typedef std::packaged_task<Result()> Task;

				if (invalid()) {
					auto promise = std::make_shared<std::promise<Result>>();
					
					std::future<Result> fut = promise->get_future();
					promise->set_exception(std::make_exception_ptr(std::runtime_error("nullptr object wrapped with Concurrent")));

					return fut;
				}

				auto bound = std::bind(fn, mWrappedObject.get(), std::forward<Args>(args)...);
				Task task(std::move(bound));
				
				std::future<Result> result = task.get_future();
				mTaskQueue.push(MoveOnCopy<Task>(std::move(task)));

				return result;
			}


		private:
			mutable std::unique_ptr<T> mWrappedObject; // Hrm... this could possibly be turned into a UniqueResource
			mutable ConcurrentQueue<detail::Task> mTaskQueue;
			std::thread mThread;
			bool mDone = false;
		};
	}
}

#endif