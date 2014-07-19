#ifndef OVERDRIVE_RENDER_CAMERA_CAMERA_BASE_H
#define OVERDRIVE_RENDER_CAMERA_CAMERA_BASE_H

#include "opengl.h"
#include "math/conversions.h"

namespace overdrive {
	namespace render {
		namespace camera {
			// Base camera (somewhat geared towards perspective projection)
			class CameraBase {
			public:
				CameraBase();

				virtual void update() = 0;
				virtual void rotate(float yaw, float pitch, float roll) = 0; // this is a rotation relative to the current orientation

				void setPosition(glm::vec3 position);
				void setPosition(float x, float y, float z);
				glm::vec3 getPosition() const;

				void setForward(glm::vec3 forward);
				void setForward(float x, float y, float z);
				glm::vec3 getForward() const;
				
				void setRight(glm::vec3 right);
				void setRight(float x, float y, float z);
				glm::vec3 getRight() const;

				void setUp(glm::vec3 up);
				void setUp(float x, float y, float z);
				glm::vec3 getUp() const;

				void setLookAt(glm::vec3 target);
				void setLookAt(float x, float y, float z);
				glm::vec3 getLookAt() const;

				glm::mat4 getView() const;

				void setProjection(glm::mat4 projection); // manual adjustment of the projection
				glm::mat4 getProjection() const;

				float getFoV() const;
				float getAspectRatio() const;
				float getNearClip() const;
				float getFarClip() const;

			protected:
				float mNearClip     = 0.1f;
				float mFarClip      = 1000.0f;
				float mFieldOfView  = math::toRadians(70.0f);
				float mAspectRatio  = 4.0f / 3.0f;
				float mLookAtDistance = 1.0f;

				glm::vec3 mForward  = glm::vec3(0.0f, 0.0f, 1.0f);
				glm::vec3 mUp       = glm::vec3(0.0f, 1.0f, 0.0f);
				glm::vec3 mRight    = glm::vec3(1.0f, 0.0f, 0.0f);

				glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, -5.0f);

				glm::mat4 mView;
				glm::mat4 mProjection;
			};
		}
	}
}

#endif
