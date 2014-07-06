#include "core/logger.h"
#include "core/channel.h"
#include "input/joystick.h"

namespace overdrive {
	namespace test {
		namespace input {
			struct JoyRecv :
				public overdrive::core::MessageHandler<overdrive::input::Joystick::OnButtonPress>,
				public overdrive::core::MessageHandler<overdrive::input::Joystick::OnMove>,
				public overdrive::core::MessageHandler<overdrive::input::Joystick::OnConnect>
			{
				void operator()(const overdrive::input::Joystick::OnButtonPress& bp) {
					gLog << "joystick button: " << bp.mButtonID;
				}

				void operator()(const overdrive::input::Joystick::OnMove& jm) {
					std::stringstream sstr;

					for (const auto& val : jm.mPosition)
						sstr << val << ", ";

					gLog << "joystick " << jm.mJoystickID << " move: " << sstr.str();
				}

				void operator()(const overdrive::input::Joystick::OnConnect& jc) {
					gLog << "Joystick " << jc.mJoystickID << " connected: " << jc.mJoystickName;
				}
			};
		}
	}
}