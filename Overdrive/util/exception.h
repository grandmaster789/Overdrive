#ifndef OVERDRIVE_UTIL_EXCEPTION_H
#define OVERDRIVE_UTIL_EXCEPTION_H

#include <string>
#include <exception>

namespace overdrive {
	class FileException : public std::exception {
	public:
		virtual const char* what() const throw();
	};

	class UnsupportedFormatException : public std::exception {
	public:
		virtual const char* what() const throw();
	};

	class VersionException : public std::exception {
	public:
		virtual const char* what() const throw();
	};

	class PixelFormatException : public std::exception {
	public:
		virtual const char* what() const throw();
	};

	class UnsupportedOperationException : public std::exception {
	public:
		UnsupportedOperationException(const std::string& description);

		virtual const char* what() const throw();

	private:
		std::string mDescription;
	};
}

#endif
