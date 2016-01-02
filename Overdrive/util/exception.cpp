#include "exception.h"
#include "../core/logger.h"

namespace overdrive {
	const char* FileException::what() const {
		gLogError << "File exception thrown";
		return "File exception thrown";
	}

	const char* UnsupportedFormatException::what() const {
		gLogError << "Unsupported format exception thrown";
		return "Unsupported format exception thrown";
	}

	const char* VersionException::what() const {
		gLogError << "Version exception thrown";
		return "Version exception thrown";
	}

	const char* PixelFormatException::what() const {
		gLogError << "Pixel format exception thrown -- no pixel format could be found that supports the specified buffer depths (and antialiasing settings)";
		return "Pixel format exception thrown";
	}

	UnsupportedOperationException::UnsupportedOperationException(const std::string& description) :
		mDescription(description)
	{
	}

	const char* UnsupportedOperationException::what() const {
		std::string message = std::string("Unsupported operation exception thrown : ") + mDescription;

		gLogError << message;

		return message.c_str();
	}
}
