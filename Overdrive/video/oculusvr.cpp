#include "stdafx.h"
#include "oculusvr.h"
#include "../core/logger.h"

namespace overdrive {
	namespace video {
		OculusVR::OculusVR() {
			auto initResult = ovr_Initialize(nullptr);

			if (!OVR_SUCCESS(initResult)) {
				ovrErrorInfo err;
				ovr_GetLastErrorInfo(&err);

				gLogError << "Failed to initialize libOVR: " << err.ErrorString; 
				return;
			}
			
			auto createResult = ovr_Create(&mSession, &mGraphicsAdapter);
			if (!OVR_SUCCESS(createResult)) {
				ovrErrorInfo err;
				ovr_GetLastErrorInfo(&err);

				gLogError << "Failed to create a new VR session: " << err.ErrorString;
				return;
			}

			mDescription = ovr_GetHmdDesc(mSession);

			gLog << "Detected HMD: " << mDescription.Type << " " << mDescription.Resolution;
		}

		OculusVR::~OculusVR() {
			ovr_Shutdown();
		}
	}
}

std::ostream& operator << (std::ostream& os, const ovrHmdType& type) {
	switch (type) {
	case ovrHmd_None: os << "None"; break;
	case ovrHmd_DK1: os << "Oculus DK1"; break;
	case ovrHmd_DK2: os << "Oculus DK2"; break;
		
	// the rest are internally released types by Oculus, so I'm not entirely sure what actual models they're supposed to be
	case ovrHmd_DKHD: os << "?Oculus DK HD?"; break;
	case ovrHmd_CB: os << "?Oculus CB?"; break;
	case ovrHmd_Other: os << "?Oculus other?"; break;
	case ovrHmd_E3_2015: os << "?Oculus E3 2015?"; break;
	case ovrHmd_ES06: os << "?Oculus ES06?"; break;
	case ovrHmd_ES09: os << "?Oculus ES09?"; break;

	default:
		os << "Unknown HMD type: " << static_cast<std::underlying_type_t<ovrHmdType>>(type);
	}

	return os;
}

std::ostream& operator << (std::ostream& os, const ovrFovPort& port) {
	os << "FOV port"
		<< "(up: " << port.UpTan
		<< ", down: " << port.DownTan
		<< ", left: " << port.LeftTan
		<< ", right: " << port.RightTan
		<< ")";

	return os;
}

std::ostream& operator << (std::ostream& os, const ovrSizei& size) {
	os << "(" << size.w << ", " << size.h << ")";
	return os;
}
