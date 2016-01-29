#pragma once

#include "component.h"
#include "../opengl.h"

namespace overdrive {
	namespace scene {
		class TransformComponent:
			public Component
		{
		public:
			TransformComponent();

			void setPosition(const glm::vec3& position);
			void setOrientation(const glm::quat& orientation);
			void setScale(const glm::vec3& scale);

			const glm::vec3& getPosition() const;
			const glm::quat& getOrientation() const;
			const glm::vec3& getScale();

			const glm::mat4& getTransform(); 

		private:
			glm::quat mOrientation;
			glm::vec3 mPosition;
			glm::vec3 mScale;

			glm::mat4 mTransform;

			bool mDirty;
		};
	}
}
