#include "log_message.h"
#include "logger.h"
#include <utility>

namespace overdrive {
	namespace core {
		LogMessage::LogMessage(Logger* owner, eLogLevel level, const std::string& file, int line):
			mOwner(owner),
			mMeta{ level, file, line }
		{
		}

		LogMessage::LogMessage(LogMessage&& lm) :
			mOwner(std::move(lm.mOwner)),
			mBuffer(std::move(lm.mBuffer)),
			mMeta(std::move(lm.mMeta))
		{
			lm.mOwner = nullptr;
		}

		LogMessage::~LogMessage() {
			if (mOwner)
				mOwner->flush(this);
		}

		LogMessage& LogMessage::operator<<(std::ostream& (*fn)(std::ostream&)) {
			fn(mBuffer);
			return *this;
		}
	}
}


