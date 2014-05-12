#include "settings.h"
#include "logger.h"

#include <fstream>

namespace overdrive {
	namespace core {
		namespace po = boost::program_options;

		Settings::Settings():
			util::Named("Settings")
		{
		}

		po::options_description& Settings::options() {
			return mOptions;
		}

		bool Settings::load(std::string filename) {
			std::ifstream ifs(filename.c_str());
			ifs.sync_with_stdio(false);

			if (!ifs.good()) {
				gLogSev(ERROR) << "Failed to open config file: " << filename;
				return false;
			}
			else {
				po::store(po::parse_config_file(ifs, mOptions, true), mVariables);
				po::notify(mVariables);

				return true;
			}
		}

		void Settings::parseCommandline(int argc, char* argv[]) {
			po::store(
				po::parse_command_line(argc, argv, mOptions),
				mVariables
			);
			mVariables.notify();
		}

		bool Settings::isAvailable(std::string option_identifier) const {
			return (mVariables.count(option_identifier) > 0);
		}
	}
}