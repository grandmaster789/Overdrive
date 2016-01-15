#pragma once

#include "../opengl.h"
#include <ostream>

namespace overdrive {
	namespace math {
		/*
			[NOTE] the classify operation is subject to floating point stability issues!
					I'm using an epsilon approach, which honestly is not ideal. It is pretty fast though.
		*/
		class Plane {
		public:
			enum class eClassification {
				FRONT,
				BACK,
				COPLANAR
			};

			Plane(); // defaults to the XZ plane
			Plane(const glm::vec3& normal, float distance);
			Plane(const glm::vec3& normal, const glm::vec3& pointOnPlane);
			Plane(const glm::vec3& pt1, const glm::vec3& pt2, const glm::vec3& pt3);

			eClassification classify(const glm::vec3& pt);
			
			const glm::vec3& getNormal() const;
			float getDistance() const;
			float getDistance(const glm::vec3& pt) const;

		private:
			glm::vec3 mNormal;
			float mDistance;
		};

		std::ostream& operator << (std::ostream& os, const Plane::eClassification& classification);
	}
}
