#include "stdafx.h"

#if OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_WINDOWS
	#pragma warning(push)
	#pragma warning(disable: 4189) // local variable is initialized but not referenced
	#pragma warning(disable: 4244) // conversion from XX to YY, possible loss of data
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

#if OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_WINDOWS
	#pragma warning(pop)
#endif

