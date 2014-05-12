#ifndef OVERDRIVE_PREPROCESSOR_H
#define OVERDRIVE_PREPROCESSOR_H

// first lay down some constants
#define OVERDRIVE_PLATFORM_WINDOWS 1
#define OVERDRIVE_PLATFORM_LINUX 2

// next, try to autodetect the platform
#ifdef _WIN32
	#define OVERDRIVE_PLATFORM OVERDRIVE_PLATFORM_WINDOWS
#else
	#define OVERDRIVE_PLATFORM OVERDRIVE_PLATFORM_LINUX
#endif

// now, perform platform-specific fixes
#if OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define WIN32_EXTRA_LEAN
	#define NO_MINMAX

	#include <windows.h>
	#include <winsdkver.h> // this defines the _WIN32_WINNT value (relevant for boost::asio)
#endif

// with MSVC we can figure out wheter this is a debug build
#ifdef _DEBUG
	#define OVERDRIVE_DEBUG
#endif

#ifdef min
	#undef min
#endif

#ifdef max
	#undef max
#endif

#endif
