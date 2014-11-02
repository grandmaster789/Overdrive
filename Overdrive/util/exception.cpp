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
}
