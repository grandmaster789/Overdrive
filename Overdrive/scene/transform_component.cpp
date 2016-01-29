#include "stdafx.h"
#include "transform_component.h"

namespace overdrive {
	namespace scene {
		TransformComponent::TransformComponent():
			mOrientation(1.0f, 0.0f, 0.0f, 0.0f),
			mPosition(0.0f, 0.0f, 0.0f),
			mScale(1.0f, 1.0f, 1.0f),
			
			mTransform(), // this yields an identity matrix
			
			mDirty(false)
		{
		}

		void TransformComponent::setPosition(const glm::vec3& position) {
			mPosition = position;
			mDirty = true;
		}

		void TransformComponent::setOrientation(const glm::quat& orientation) {
			mOrientation = orientation;
			mDirty = true;
		}

		void TransformComponent::setScale(const glm::vec3& scale) {
			mScale = scale;
			mDirty = true;
		}

		const glm::vec3& TransformComponent::getPosition() const {
			return mPosition;
		}

		const glm::quat& TransformComponent::getOrientation() const {
			return mOrientation;
		}

		const glm::vec3& TransformComponent::getScale() {
			return mScale;
		}

		const glm::mat4& TransformComponent::getTransform() {
			if (mDirty) {
				auto rotationMatrix = glm::mat4_cast(mOrientation);
				auto translationMatrix = glm::translate(mPosition);
				auto scaleMatrix = glm::scale(mScale);

				mTransform = scaleMatrix * rotationMatrix * translationMatrix;
			}
			
			return mTransform;
		}
	}
}