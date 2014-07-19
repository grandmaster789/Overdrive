#include "render/camera/cameraBase.h"

namespace overdrive {
	namespace render {
		namespace camera {
			CameraBase::CameraBase() {
			}

			void CameraBase::setPosition(glm::vec3 position) {
				mPosition = position;
			}

			void CameraBase::setPosition(float x, float y, float z) {
				mPosition.x = x;
				mPosition.y = y;
				mPosition.z = z;
			}

			glm::vec3 CameraBase::getPosition() const {
				return mPosition;
			}

			void CameraBase::setForward(glm::vec3 forward) {
				mForward = forward;
			}

			void CameraBase::setForward(float x, float y, float z) {
				mForward.x = x;
				mForward.y = y;
				mForward.z = z;
			}

			glm::vec3 CameraBase::getForward() const {
				return mForward;
			}

			void CameraBase::setRight(glm::vec3 right) {
				mRight = right;
			}

			void CameraBase::setRight(float x, float y, float z) {
				mRight.x = x;
				mRight.y = y;
				mRight.z = z;
			}

			glm::vec3 CameraBase::getRight() const {
				return mRight;
			}

			void CameraBase::setUp(glm::vec3 up) {
				mUp = up;
			}

			void CameraBase::setUp(float x, float y, float z) {
				mUp.x = x;
				mUp.y = y;
				mUp.z = z;
			}

			glm::vec3 CameraBase::getUp() const {
				return mUp;
			}

			void CameraBase::setLookAt(glm::vec3 target) {
				mForward = (target - mPosition);
				mLookAtDistance = static_cast<float>(mForward.length());
				glm::normalize(mForward);
			}

			void CameraBase::setLookAt(float x, float y, float z) {
				setLookAt(glm::vec3(x, y, z));
			}

			glm::vec3 CameraBase::getLookAt() const {
				return mForward * mLookAtDistance;
			}

			glm::mat4 CameraBase::getView() const {
				return mView;
			}

			void CameraBase::setProjection(glm::mat4 projection) {
				mProjection = projection;
			}

			glm::mat4 CameraBase::getProjection() const {
				return mProjection;
			}

			float CameraBase::getFoV() const {
				return mFieldOfView;
			}

			float CameraBase::getAspectRatio() const {
				return mAspectRatio;
			}

			float CameraBase::getNearClip() const {
				return mNearClip;
			}

			float CameraBase::getFarClip() const {
				return mFarClip;
			}
		}
	}
}