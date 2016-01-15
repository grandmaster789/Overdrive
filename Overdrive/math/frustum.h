#pragma once

#include "plane.h"
#include "intersection.h"
#include <ostream>

namespace overdrive {
	namespace math {
		enum class eCubePlane: int {
			FRONT,
			BACK,
			TOP,
			BOTTOM,
			LEFT,
			RIGHT
		};

		class Frustum {
		public:
			Frustum(); // defaults to orthogonal cube with planes at +1 and -1 from the origin

			const Plane& getPlane(eCubePlane id) const;

		protected:
			Plane mPlanes[6];
		};

		std::ostream& operator << (std::ostream& os, const eCubePlane& plane);
	}
}
