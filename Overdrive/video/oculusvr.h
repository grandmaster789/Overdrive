#pragma once

#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>
#include <OVR_ErrorCode.h>

namespace overdrive {
	namespace video {
		/*
			Helper class that initializes LibOVR, enumerates and provides access and information for supported
			HMD devices (DK1 / DK2 / CV1 etc)

			Mainly tested with Oculus SDK 0.8.0 and a DK2

			[TODO] Test with more stuff, update to SDK 1.0 when it becomes widely available
			[TODO] This seems like it'll be not-so-ideal with multi-gpu systems... when I have one, test what happens
		*/
		class OculusVR {
		public:
			OculusVR();
			~OculusVR();

		private:
			ovrSession mSession;
			ovrGraphicsLuid mGraphicsAdapter;
			ovrHmdDesc mDescription;
		};
	}
}

std::ostream& operator << (std::ostream& os, const ovrHmdType& type);
std::ostream& operator << (std::ostream& os, const ovrFovPort& port);
std::ostream& operator << (std::ostream& os, const ovrSizei& size);