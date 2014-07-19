#ifndef OVERDRIVE_MATH_PLANE_H
#define OVERDRIVE_MATH_PLANE_H

#include "opengl.h"
#include "math/constants.h"

namespace overdrive {
	namespace math {
		class Plane {
		public:
			enum class eClassification {
				FRONT,
				BACK,
				ON_PLANE
			};

			Plane();
			Plane(glm::vec3 normal, glm::vec3 position);
			Plane(glm::vec3 normal, float distance);
			
			static Plane fromPoints(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

			eClassification classify(glm::vec3 point) const;

			float getDistance(glm::vec3 point) const;

		private:
			glm::vec3 mNormal;
			float mDistance; //distance to origin
		};

		//------------------------ IMPLEMENTATION ------------------------//
		
		Plane::Plane():
			mNormal(0, 1, 0),
			mDistance(0)
		{
		}

		Plane::Plane(glm::vec3 normal, glm::vec3 position) :
			mNormal(normal)
		{
			mDistance = -glm::dot(mNormal, position);
		}

		Plane::Plane(glm::vec3 normal, float distance) :
			mNormal(normal),
			mDistance(distance)
		{
		}

		Plane Plane::fromPoints(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
			glm::vec3 a = p2 - p1;
			glm::vec3 b = p3 - p1;

			glm::vec3 normal = glm::cross(a, b);
			glm::normalize(normal);

			return Plane(normal, p2);
		}

		Plane::eClassification Plane::classify(glm::vec3 point) const {
			float distance = getDistance(point);
			
			if (distance > EPSILON)
				return eClassification::FRONT;

			if (distance < -EPSILON)
				return eClassification::BACK;

			return eClassification::ON_PLANE;
		}

		float Plane::getDistance(glm::vec3 point) const {
			return glm::dot(mNormal, point) + mDistance;
		}
	}
}

#endif
