#include "stdafx.h"
#include "log_message.h"
#include "../logger.h"

namespace overdrive {
	namespace core {
		LogMessage::LogMessage(
			eLogLevel level,
			const std::string& file,
			int line,
			Logger* owner
		):
			mOwner(owner)
		{
			mMeta = {
				level,
				file,
				line
			};
		}

		LogMessage::~LogMessage() {
			if (mOwner && logLevel(mMeta.mLevel))
				mOwner->flush(*this);
		}

		LogMessage::LogMessage(LogMessage&& message) :
			mMeta(std::move(message.mMeta)),
			mOwner(std::move(message.mOwner)),
			mBuffer(std::move(message.mBuffer))
		{
			message.mOwner = nullptr;
		}

		LogMessage& LogMessage::operator << (std::ostream& (*fn)(std::ostream& os)) {
			fn(mBuffer);
			return *this;
		}
	}
}