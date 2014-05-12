#ifndef OVERDRIVE_CORE_SETTINGS_H
#define OVERDRIVE_CORE_SETTINGS_H

#include "util/named.h"

#include <string>
#include <boost/program_options.hpp>
#include <boost/noncopyable.hpp>

namespace overdrive {
	namespace core {
		/*
			Wrapper around boost::program_options, for super-easy system/module configuration
		*/
		class Settings :
			public util::Named,
			public boost::noncopyable
		{
		private:
			typedef boost::program_options::options_description Options;
			typedef boost::program_options::variables_map Variables;

		public:
			Settings();
			
			bool load(std::string filename); // TODO: this really should be some type of path name
			void parseCommandline(int argc, char* argv[]);

			bool isAvailable(std::string option_identifier) const;

			Options& options();

			template <typename T>
			T get(std::string option_identifier) const;

		private:
			Options mOptions;
			Variables mVariables;
		};

		template <typename T>
		T Settings::get(std::string option_identifier) const {
			if (isAvailable(option_identifier))
				return mVariables[option_identifier.c_str()].as<T>;
			else
				throw std::runtime_error(std::string("Failed to find setting option: ") + option_identifier);
		}
	}
}

#endif
