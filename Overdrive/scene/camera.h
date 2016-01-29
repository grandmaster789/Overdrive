#pragma once

#include "entity.h"
#include "../opengl.h"
#include "../math/frustum.h"
#include "../core/channel.h"
#include "../video/window.h"
#include <ostream>

namespace overdrive {
	namespace scene {
		// [NOTE] perhaps decouple the viewport stuff?
		// [NOTE] perhaps use a dirty flag and always call update?
		// [NOTE] add convenience stuff for offscreen rendering
		// [NOTE] using transposed operations makes the expressions neater, but increases the amount of actual work done... minor performance consideration, that
		class Camera:
			public Entity,
			public MessageHandler<video::Window::OnFramebufferResized>
		{
		public:
			enum class eMode {
				PERSPECTIVE,
				ORTHO
			};

			Camera(); // defaults to orthogonal projection

			void setProjection(
				float fovY, 
				float aspectRatio, 
				float nearClip, 
				float farClip
			);

			void setOrtho();

			void lookAt(const glm::vec3& pt); // [NOTE] the up vector is derived from the current orientation
			
			void setPosition(const glm::vec3& pos);
			void setPosition(float x, float y, float z);
			void setOrientation(const glm::quat& q);
			const glm::vec3& getPosition() const;
			const glm::quat& getOrientation() const;
			
			void setForward(const glm::vec3& fwd);
			void setUp(const glm::vec3& up);
			void setRight(const glm::vec3& right);
			glm::vec3 getForward() const;
			glm::vec3 getUp() const;
			glm::vec3 getRight() const;

			struct CameraAxes {
				glm::vec3 mForward;
				glm::vec3 mUp;
				glm::vec3 mRight;
			};
			CameraAxes getDirections() const; // yields forward + up + right, in that order

			void setFOV(float radians);
			void setAspect(float ratio);
			void setClip(float nearClip, float farClip);
			
			void setViewport(int x, int y, int width, int height);
			void setViewportWindow(video::Window* window); // uses the entire window, derives aspect ratio from the window
			
			eMode getMode() const;
			const glm::mat4& getView() const;
			const glm::mat4& getProjection() const;

			void update(); // sets the viewport and recalculates matrices

			void operator()(const video::Window::OnFramebufferResized& resized);

		protected:
			eMode mCameraMode;

			// position/orientation
			glm::vec3 mPosition;
			glm::quat mOrientation;		
			
			// viewport-related
			video::Window* mWindow;
			int mViewportX;
			int mViewportY;
			int mViewportWidth;
			int mViewportHeight;

			// perspective
			float mFOVy;
			float mAspectRatio;
			float mNearClip;
			float mFarClip;

			// matrices
			glm::mat4 mView;
			glm::mat4 mProjection;

			// frustum
			math::Frustum mFrustum;
		};

		std::ostream& operator << (std::ostream& os, const Camera::eMode& mode);
	}
}
