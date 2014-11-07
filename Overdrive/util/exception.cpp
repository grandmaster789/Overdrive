#include "util/exception.h"
#include "core/logger.h"

namespace overdrive {
	const char* FileException::what() const {
		gLog.error() << "File exception thrown";
		return "File exception thrown";
	}

	const char* UnsupportedFormatException::what() const {
		gLog.error() << "Unsupported format exception thrown";
		return "Unsupported format exception thrown";
	}

	const char* VersionException::what() const {
		gLog.error() << "Version exception thrown";
		return "Version exception thrown";
	}
	
	const char* PixelFormatException::what() const {
		gLog.error() << "Pixel format exception thrown -- no pixel format could be found that supports the specified buffer depths (and antialiasing settings)";
		return "Pixel format exception thrown";
	}
}
