#include "stdafx.h"
#include "plane.h"

namespace overdrive {
	namespace math {
		namespace {
			float gEpsilon = 0.0001f;
		}

		Plane::Plane():
			mNormal(0, 1, 0),
			mDistance(0)
		{
		}

		Plane::Plane(
			const glm::vec3& normal,
			float distance
		):
			mNormal(normal),
			mDistance(distance)
		{
		}

		Plane::Plane(
			const glm::vec3& normal, 
			const glm::vec3& pointOnPlane
		):
			mNormal(normal)
		{
			mDistance = -glm::dot(normal, pointOnPlane);
		}

		Plane::Plane(
			const glm::vec3& pt1, 
			const glm::vec3& pt2, 
			const glm::vec3& pt3
		) {
			glm::vec3 a = pt2 - pt1;
			glm::vec3 b = pt3 - pt1;

			mNormal = glm::normalize(glm::cross(a, b));
			mDistance = -glm::dot(mNormal, pt2);
		}

		Plane::eClassification Plane::classify(const glm::vec3& pt) {
			auto distance = getDistance(pt);

			if (distance > gEpsilon)
				return eClassification::FRONT;

			if (distance < -gEpsilon)
				return eClassification::BACK;

			return eClassification::COPLANAR;
		}

		const glm::vec3& Plane::getNormal() const {
			return mNormal;
		}

		float Plane::getDistance() const {
			return mDistance;
		}

		float Plane::getDistance(const glm::vec3& pt) const {
			return glm::dot(mNormal, pt) + mDistance;
		}

		std::ostream& operator << (std::ostream& os, const Plane::eClassification& classification) {
			switch (classification) {
			case Plane::eClassification::FRONT: 
				os << "Front"; 
				break;

			case Plane::eClassification::BACK:
				os << "Back"; 
				break;

			case Plane::eClassification::COPLANAR: 
				os << "Coplanar"; 
				break;

			default:
				os << "Unsupported classification: " << static_cast<std::underlying_type_t<Plane::eClassification>>(classification);
			}

			return os;
		}
	}
}