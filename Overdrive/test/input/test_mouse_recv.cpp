#include "core/logger.h"
#include "core/channel.h"
#include "input/mouse.h"

namespace overdrive {
	namespace test {
		namespace input {
			struct MouseRecv :
				public overdrive::core::MessageHandler<overdrive::input::Mouse::OnButtonPress>,
				public overdrive::core::MessageHandler<overdrive::input::Mouse::OnMove>,
				public overdrive::core::MessageHandler<overdrive::input::Mouse::OnEnter>,
				public overdrive::core::MessageHandler<overdrive::input::Mouse::OnLeave>,
				public overdrive::core::MessageHandler<overdrive::input::Mouse::OnScroll>
			{
				void operator()(const overdrive::input::Mouse::OnButtonPress& mbp) {
					gLog << "mouse press: " << mbp.mButton;
				}

				void operator()(const overdrive::input::Mouse::OnMove& mm) {
					gLog << "move: " <<
						" (" << mm.mX << ", " << mm.mY << ") "
						" (" << mm.mDeltaX << ", " << mm.mDeltaY << ")";
				}

				void operator()(const overdrive::input::Mouse::OnEnter&) {
					gLog << "mouse entered";
				}

				void operator()(const overdrive::input::Mouse::OnLeave&) {
					gLog << "mouse left";
				}

				void operator()(const overdrive::input::Mouse::OnScroll& scr) {
					gLog << "mouse scroll " << scr.mXOffset << ", " << scr.mYOffset;
				}
			};
		}
	}
}