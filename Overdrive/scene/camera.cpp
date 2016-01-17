#include "stdafx.h"
#include "camera.h"
#include "../core/logger.h"
#include <boost/math/constants/constants.hpp>
#include <tuple>

namespace overdrive {
	namespace scene {
		namespace {
			const float PI = boost::math::float_constants::pi;
		}

		Camera::Camera():
			mCameraMode(eMode::ORTHO),

			mPosition(0.0f, 0.0f, 0.0f),
			mOrientation(1.0f, 0.0f, 0.0f, 0.0f), // [NOTE] glm does WXYZ for quaternions...
			
			mWindow(nullptr),
			mViewportX(0),
			mViewportY(0),
			mViewportWidth(800),
			mViewportHeight(600),

			mFOVy(PI * 0.25f),
			mAspectRatio(16.0f / 9.0f), // for a while now this is the default for monitors (720p, 1080p, 4k etc are all 16:9)
			mNearClip(0.1f),
			mFarClip(100.0f)
		{
		}

		void Camera::setProjection(
			float fovY,
			float aspectRatio,
			float nearClip,
			float farClip
		) {
			mCameraMode = eMode::PERSPECTIVE;

			mFOVy = fovY;
			mAspectRatio = aspectRatio;
			mNearClip = nearClip;
			mFarClip = farClip;
		}

		void Camera::setOrtho() {
			mCameraMode = eMode::ORTHO;
		}

		void Camera::lookAt(const glm::vec3& pt) {
			glm::mat4 m = glm::lookAt(
				mPosition,
				pt,
				getUp() // re-use the current up orientation
			);
			
			// convert back into a new orientation (discarding position)
			mOrientation = glm::quat_cast(m);
		}

		void Camera::setPosition(const glm::vec3& pos) {
			mPosition = pos;
		}

		void Camera::setPosition(float x, float y, float z) {
			mPosition = glm::vec3(x, y, z);
		}

		const glm::vec3& Camera::getPosition() const {
			return mPosition;
		}

		void Camera::setOrientation(const glm::quat& q) {
			mOrientation = q;
		}

		const glm::quat& Camera::getOrientation() const {
			return mOrientation;
		}

		void Camera::setForward(const glm::vec3& fwd) {
			auto rotationMatrix = glm::mat3_cast(mOrientation);

			rotationMatrix[2] = fwd;

			mOrientation = glm::quat_cast(rotationMatrix);
		}

		void Camera::setUp(const glm::vec3& up) {
			auto rotationMatrix = glm::mat3_cast(mOrientation);

			rotationMatrix[1] = up;

			mOrientation = glm::quat_cast(rotationMatrix);
		}

		void Camera::setRight(const glm::vec3& right) {
			auto rotationMatrix = glm::mat3_cast(mOrientation);

			rotationMatrix[0] = right;

			mOrientation = glm::quat_cast(rotationMatrix);
		}

		glm::vec3 Camera::getForward() const {
			auto rotationMatrix = glm::mat3_cast(mOrientation);

			return rotationMatrix[2];
		}

		glm::vec3 Camera::getUp() const {
			auto rotationMatrix = glm::mat3_cast(mOrientation);
			
			return rotationMatrix[1];
		}

		glm::vec3 Camera::getRight() const {
			auto rotationMatrix = glm::mat3_cast(mOrientation);

			return rotationMatrix[0];
		}

		std::tuple<glm::vec3, glm::vec3, glm::vec3> Camera::getDirections() const {
			auto rotationMatrix = glm::mat3_cast(mOrientation);

			return std::make_tuple(
				rotationMatrix[2],	// forward
				rotationMatrix[1],	// up
				rotationMatrix[0]	// right
			);
		}

		void Camera::setFOV(float radians) {
			mFOVy = radians;
		}

		void Camera::setAspect(float ratio) {
			mAspectRatio = ratio;
		}

		void Camera::setClip(float nearClip, float farClip) {
			mNearClip = nearClip;
			mFarClip = farClip;
		}

		void Camera::setViewport(int x, int y, int width, int height) {
			mViewportX = x;
			mViewportY = y;
			mViewportWidth = width;
			mViewportHeight = height;
		}

		void Camera::setViewportWindow(video::Window* window) {
			using std::tie;

			if (window) {
				mWindow = window;

				mViewportX = 0;
				mViewportY = 0;

				tie(mViewportWidth, mViewportHeight) = window->getFramebufferSize();

				mAspectRatio = static_cast<float>(mViewportWidth) / static_cast<float>(mViewportHeight);
			}
			else {
				mWindow = nullptr;

				mViewportX = 0;
				mViewportY = 0;
				mViewportWidth = 800;
				mViewportHeight = 600;
			}
		}

		Camera::eMode Camera::getMode() const {
			return mCameraMode;
		}

		const glm::mat4& Camera::getView() const {
			return mView;
		}

		const glm::mat4& Camera::getProjection() const {
			return mProjection;
		}

		void Camera::update() {
			// set viewport
			glViewport(
				mViewportX, 
				mViewportY, 
				mViewportWidth, 
				mViewportHeight
			);

			// update view matrix
			mView = glm::mat4_cast(mOrientation) * glm::translate(-mPosition);

			// update projection matrix
			switch (mCameraMode) {
			case eMode::ORTHO:
				mProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
				break;

			case eMode::PERSPECTIVE:
				mProjection = glm::perspective(mFOVy, mAspectRatio, mNearClip, mFarClip);
				break;

			default:
				assert(false);
			}
		}

		void Camera::operator()(const video::Window::OnFramebufferResized& resized) {
			if (resized.mWindow == mWindow) {
				mViewportWidth = resized.mWidth;
				mViewportHeight = resized.mHeight;

				mAspectRatio = static_cast<float>(resized.mWidth) / static_cast<float>(resized.mHeight);
			}
		}

		std::ostream& operator << (std::ostream& os, const Camera::eMode& mode) {
			switch (mode) {
			case Camera::eMode::PERSPECTIVE:
				os << "Perspective";
				break;

			case Camera::eMode::ORTHO:
				os << "Ortho";
				break;

			default:
				os << "Unknown camera mode: " << static_cast<std::underlying_type_t<Camera::eMode>>(mode);
			}

			return os;
		}
	}
}