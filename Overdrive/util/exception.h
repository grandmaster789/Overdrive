#pragma once

#include <string>
#include <exception>

namespace overdrive {
	struct FileException:
		public std::exception 
	{
		virtual const char* what() const throw();
	};

	struct UnsupportedFormatException: 
		public std::exception 
	{
		virtual const char* what() const throw();
	};

	struct VersionException: 
		public std::exception 
	{
		virtual const char* what() const throw();
	};

	struct PixelFormatException: 
		public std::exception 
	{
		virtual const char* what() const throw();
	};

	struct UnsupportedOperationException: 
		public std::exception 
	{
		UnsupportedOperationException(const std::string& description);

		virtual const char* what() const throw();

	private:
		std::string mDescription;
	};
}
