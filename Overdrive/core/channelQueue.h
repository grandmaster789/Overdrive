#ifndef OVERDRIVE_CORE_CHANNELQUEUE_H
#define OVERDRIVE_CORE_CHANNELQUEUE_H

#include <algorithm>
#include <functional>
#include <mutex>
#include <utility>
#include <vector>

namespace overdrive {
	namespace core {
		class Channel;

		namespace detail {
			template <typename tEvent>
			class ChannelQueue {
			private:					// entire class is private
				friend class Channel;	// only to be used by Channel

				typedef std::mutex Mutex;	// I'm actually expecting low contention, so this is a candidate for replacement with spinlocks
				typedef std::lock_guard<Mutex> ScopedLock;

				static ChannelQueue& instance() {
					static ChannelQueue aSpecificInstance;
					return aSpecificInstance;
				}

				template <typename tHandler>
				void add(tHandler* handlerObject) {
					ScopedLock lock(mMutex);

					mHandlers.push_back(
						std::make_pair(
							[handlerObject] (const tEvent& message) { 
								(*handlerObject)(message); 
							},
							handlerObject
						)
					);
				}

				void remove(void* objectPtr) {
					ScopedLock lock(mMutex);

					auto it = std::remove_if(
						mHandlers.begin(),
						mHandlers.end(),
						[objectPtr](const HandlerPair& handlerPair) {
							return handlerPair.second == objectPtr;
						}
					);

					if (it != mHandlers.end())
						mHandlers.erase(it);
				}

				void broadcast(const tEvent& message) const {
					std::vector<HandlerPair> handlers;
					
					{
						ScopedLock lock(mMutex);

						// make a local copy so that we have a non-mutable structure to iterate over
						// this should be pretty fast, as it only contains PODs 
						handlers = mHandlers;	
					}

					for (const auto& handler: handlers)
						handler.first(message);
				}

			private:
				mutable std::mutex mMutex;

				typedef std::function<void(const tEvent&)> HandlerType;
				typedef std::pair<HandlerType, void*> HandlerPair; // store both the function object and the pointer to the actual handler (for removal)
				std::vector<HandlerPair> mHandlers;
			};
		}
	}
}

#endif