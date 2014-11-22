#ifndef OVERDRIVE_CORE_SYSTEM_H
#define OVERDRIVE_CORE_SYSTEM_H

#include "util/named.h"
#include "core/channel.h"
#include <boost/program_options.hpp>

namespace overdrive {
	namespace core {
		class Engine;

		// Abstract 'system' (aka module)
		class System:
			public util::Named
		{
		public:
			friend class Engine;

			typedef boost::program_options::options_description Settings;

			System(std::string name);

			System(const System&) = delete;
			System& operator = (const System&) = delete;

			virtual bool initialize();
			virtual void update();
			virtual void shutdown();

			template <typename T>
			void registerSetting(
				const std::string& nameInConfigFile, 
				T* correspondingVariable
			);

		protected:
			Channel mChannel;
			Engine* mEngine;
			Settings mSettings;
		};
	}
}

#include "system.inl"

#endif
