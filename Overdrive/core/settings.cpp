#include "settings.h"
#include "logger.h"

#include <fstream>
#include <boost/filesystem.hpp>

namespace overdrive {
	namespace core {
		namespace po = boost::program_options;

		Settings::Settings() {
		}

		po::options_description& Settings::options() {
			return mOptions;
		}

		bool Settings::load(const std::string& filename) {
			std::ifstream ifs(filename.c_str());
			ifs.sync_with_stdio(false);

			if (!ifs.good()) {
				gLogError << "Failed to open config file: " << filename;
				gLogError << "Current path: " << boost::filesystem::current_path();
				return false;
			}
			else {
				po::store(
					po::parse_config_file(ifs, mOptions, true),
					mVariables
				);
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

		bool Settings::isAvailable(const std::string& option_identifier) const {
			return (mVariables.count(option_identifier) > 0);
		}
	}
}