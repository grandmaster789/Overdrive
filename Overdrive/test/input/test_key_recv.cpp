#include "core/logger.h"
#include "core/channel.h"
#include "input/keyboard.h"

namespace overdrive {
	namespace test {
		namespace input {
			struct KeyRecv :
				public overdrive::core::MessageHandler<overdrive::input::Keyboard::OnKeyPress>,
				public overdrive::core::MessageHandler<overdrive::input::Keyboard::OnKeyRelease>
			{
				void operator()(const overdrive::input::Keyboard::OnKeyPress& kp) {
					gLog << "received key: " << kp.mKey;
				}

				void operator()(const overdrive::input::Keyboard::OnKeyRelease& kr) {
					gLog << "key released: " << kr.mKey;
				}
			};
		}
	}
}