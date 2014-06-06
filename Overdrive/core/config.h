#ifndef OVERDRIVE_CORE_CONFIG_H
#define OVERDRIVE_CORE_CONFIG_H

#include <string>
#include <boost/program_options.hpp>

#include "core/logger.h"

namespace overdrive {
	namespace core {
		class Config {
		public:
			Config();

			bool load(const std::string& filename);
			void parseCommandLine(int argc, char* argv[]);

			bool isAvailable(const std::string& identifier) const;

			boost::program_options::options_description& settings();

			template <typename T>
			T get(const std::string& identifier) const;

		private:
			boost::program_options::options_description mOptions;
			boost::program_options::variables_map mVariables;
		};

		template <typename T>
		T Config::get(const std::string& identifier) const {
			if (isAvailable(name))
				return mVariables[identifier.c_str()].as<T>();

			gLog.error() << "Failed to find variable: " << identifier;

			return T; // assumes the type can be default-constructed (maybe throw an exception instead?)
		}
	}
}

#endif
