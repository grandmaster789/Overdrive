#pragma once

#include "csv.h"
#include "tuple.h"
#include "../core/logger.h"
#include <stdexcept>
#include <fstream>

namespace overdrive {
	namespace util {
		template <typename...DataPack>
		CSV<DataPack...>::CSV(const std::string& filenaem):
			mFilename(filename)
		{
		}

		template <typename...DataPack>
		CSV<DataPack...>::CSV(const std::string& filename, std::initializer_list<std::string> headings):
			mFilename(filename)
		{
			if (headings.size() != sizeof...(DataPack))
				throw std::runtime_error("Incorrect number of headings provided");

			size_t i = 0;
			for (auto it = headings.begin(); i < headings.size(); ++i) {
				mHeadings.append(*it++);

				if (i != (headings.size() - 1))
					mHeadings.append(";");
				else
					mHeadings.append("\n");
			}
		}

		template <typename...DataPack>
		CSV<DataPack...>::~CSV() {
			std::ofstream outFile(mFilename.c_str());

			if (!outFile.good()) {
				gLogError << "Failed to open CSV output file: " << mFilename;
				return;
			}

			if (!mHeadings.empty())
				outFile << mHeadings;

			outFile << std::boolalpha;

			for (const auto& entry : mEntries) {
				size_t i = 0;
				size_t newLine = sizeof...(DataPack) - 1;

				for_each_in_tuple(
					[&](auto field) {
						outFile << field;

						if (i < newLine)
							outFile << ";";
						else
							outFile << "\n";
						
						++i;
					},
					entry
				);
			}
		}

		template <typename...DataPack>
		void CSV<DataPack...>::add(DataPack...entry) {
			mEntries.push_back(Entry(entry...));
		}
	}
}