#include "stdafx.h"
#include "videomode.h"

namespace overdrive {
	namespace video {
		VideoMode::VideoMode():
			mWidth(0),
			mHeight(0),
			mRedBits(0),
			mGreenBits(0),
			mBlueBits(0),
			mRefreshRate(0)
		{
		}

		VideoMode::VideoMode(const GLFWvidmode& source):
			mWidth(source.width),
			mHeight(source.height),
			mRedBits(source.redBits),
			mGreenBits(source.greenBits),
			mBlueBits(source.blueBits),
			mRefreshRate(source.refreshRate)
		{
		}

		VideoMode::VideoMode(const GLFWvidmode* source):
			mWidth(source->width),
			mHeight(source->height),
			mRedBits(source->redBits),
			mGreenBits(source->greenBits),
			mBlueBits(source->blueBits),
			mRefreshRate(source->refreshRate)
		{
		}
	}
}

std::ostream& operator << (std::ostream& os, const overdrive::video::VideoMode& vm) {
	os 
		<< "Video mode: " 
		<< vm.mWidth 
		<< "x" 
		<< vm.mHeight 
		<< "@" 
		<< vm.mRefreshRate;

	return os;
}