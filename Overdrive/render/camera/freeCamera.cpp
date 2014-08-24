#include "render/camera/freeCamera.h"

namespace overdrive {
	namespace render {
		namespace camera {
			FreeCamera::FreeCamera():
				mYaw(0.0f),
				mPitch(0.0f),
				mRoll(0.0f),
				mTranslation(0.0f, 0.0f, 0.0f)
			{
			}

			void FreeCamera::update() {
				glm::mat4 rotation = glm::mat4(1.0f);

				rotation = glm::rotate(rotation, mRoll,  glm::vec3(0.0f, 0.0f, 1.0f));
				rotation = glm::rotate(rotation, mYaw,   glm::vec3(0.0f, 1.0f, 0.0f));
				rotation = glm::rotate(rotation, mPitch, glm::vec3(1.0f, 0.0f, 0.0f));

				// apply and reset translation
				mPosition += mTranslation;
				mTranslation = glm::vec3(0.0f);

				mForward = glm::vec3(rotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
				mUp =      glm::vec3(rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
				mRight =   glm::vec3(rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
				
				glm::vec3 target = mPosition + mForward;

				mView = glm::lookAt(mPosition, target, mUp);
			}

			void FreeCamera::rotate(float yaw, float pitch, float roll) {
				mYaw = yaw;
				mPitch = pitch;
				mRoll = roll;
			}

			void FreeCamera::moveForward(float distance) {
				mTranslation += mForward * distance;
			}

			void FreeCamera::moveRight(float distance) {
				mTranslation += mRight * distance;
			}

			void FreeCamera::moveUp(float distance) {
				mTranslation += mUp * distance;
			}
		}
	}
}
