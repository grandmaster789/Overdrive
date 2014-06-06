#include "core/config.h"
#include <fstream>

namespace overdrive {
	namespace core {
		Config::Config() {
		}

		boost::program_options::options_description& Config::settings() {
			return mOptions;
		}

		bool Config::load(const std::string& filename) {
			std::ifstream ifs(filename.c_str());

			namespace po = boost::program_options;

			if (!ifs.good()) {
				gLog << "Failed to open config file " << filename;
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

		void Config::parseCommandLine(int argc, char* argv[]) {
			namespace po = boost::program_options;

			po::store(
				po::parse_command_line(argc, argv, mOptions),
				mVariables
			);
			po::notify(mVariables);
		}

		bool Config::isAvailable(const std::string& identifier) const {
			return (mVariables.count(identifier) > 0);
		}
	}
}