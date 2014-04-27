#ifndef OVERDRIVE_UTIL_NAMED_H
#define OVERDRIVE_UTIL_NAMED_H

#include <string>

namespace overdrive {
	namespace util {
		class Named {
		public:
			Named(std::string name);

			const std::string& getName() const;

		protected:
			void setName(std::string name);

		private:
			std::string mName;
		};
	}
}

#endif
