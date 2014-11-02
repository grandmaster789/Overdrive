#ifndef OVERDRIVE_UTIL_EXCEPTION_H
#define OVERDRIVE_UTIL_EXCEPTION_H

#include <exception>

namespace overdrive {
	class FileException :
		public std::exception
	{
	public:
		virtual const char* what() const throw();
	};

	class UnsupportedFormatException :
		public std::exception
	{
	public:
		virtual const char* what() const throw();
	};
}

#endif
