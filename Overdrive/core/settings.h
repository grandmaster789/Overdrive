#pragma once

#include <string>
#include <boost/program_options.hpp>

namespace overdrive {
	namespace core {
		/*
			Wrapper around boost::program_options, for super-easy system/module configuration
		*/
		class Settings {
		private:
			typedef boost::program_options::options_description Options;
			typedef boost::program_options::variables_map Variables;

		public:
			Settings();
			Settings(const Settings&) = delete;
			Settings& operator = (const Settings&) = delete;

			bool load(const std::string& filename); // TODO: this really should be some type of path name
			void parseCommandline(int argc, char* argv[]);

			bool isAvailable(const std::string& option_identifier) const;

			Options& options();

			template <typename T>
			T get(const std::string& option_identifier) const;

		private:
			Options mOptions;
			Variables mVariables;
		};
	}
}

#include "settings.inl"
