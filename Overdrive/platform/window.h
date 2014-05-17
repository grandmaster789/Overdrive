#ifndef OVERDRIVE_PLATFORM_WINDOW_H
#define OVERDRIVE_PLATFORM_WINDOW_H

#include "preprocessor.h"
#include <string>
#include "windowsettings.h"

namespace overdrive {
	namespace platform {
		class Window {
		public:
			Window(std::string name, WindowSettings&& settings);
		};
	}
}

#endif
