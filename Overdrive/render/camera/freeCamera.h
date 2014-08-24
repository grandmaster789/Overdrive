#ifndef OVERDRIVE_RENDER_CAMERA_FREECAMERA_H
#define OVERDRIVE_RENDEr_CAMERA_FREECAMERA_H

#include "render/camera/camerabase.h"

namespace overdrive {
	namespace render {
		namespace camera{
			class FreeCamera:
				public CameraBase
			{
			public:
				FreeCamera();
				
				virtual void update() override;
				virtual void rotate(float yaw, float pitch, float roll) override;

				// relative movement helpers
				void moveForward(float distance);
				void moveRight(float distance);
				void moveUp(float distance);

			protected:
				float mYaw;
				float mPitch;
				float mRoll;

				glm::vec3 mTranslation;
			};
		}
	}
}

#endif
