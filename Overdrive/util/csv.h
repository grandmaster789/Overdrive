#pragma once

#include <tuple>
#include <initializer_list>
#include <string>
#include <vector>

namespace overdrive {
	namespace util {
		/*
			Helper for exporting statistics to a spreadsheet-friendly format
			(CSV format, using ';' as field delimiters)
			[NOTE] optimized for convenience, not performance
			[NOTE] This relies on the existance of operator << for string conversion
				   Custom types should provide additional overloads

			Ex: 
				CSV<int, float, bool> foobar("myFilename.csv", { "sample", "value", "flag" });
				foobar.add(1, 1.0f, false);
				foobar.add(2, 3.0f, true);
		*/
		template <typename...DataPack>
		class CSV {
		public:
			typedef std::tuple<DataPack...> Entry;
			
			CSV(const std::string& filename);
			CSV(const std::string& filename, std::initializer_list<std::string> headings);
			~CSV();

			CSV(const CSV&) = delete;
			CSV(CSV&&) = delete;
			CSV& operator = (const CSV&) = delete;
			CSV& operator = (CSV&&) = delete;

			void add(DataPack...entry);
			
		private:
			std::string mFilename; // [NOTE] the filename probably should be a filesystem path
			std::string mHeadings;
			std::vector<Entry> mEntries;
		};
	}
}

#include "csv.inl"