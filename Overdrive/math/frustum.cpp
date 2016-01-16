#include "stdafx.h"
#include "frustum.h"

namespace overdrive {
	namespace math {
		Frustum::Frustum() {
		}

		const Plane& Frustum::getPlane(eCubePlane id) const {
			return mPlanes[static_cast<int>(id)];
		}

		std::ostream& operator << (std::ostream& os, const eCubePlane& plane) {
			switch (plane) {
			case eCubePlane::FRONT: 
				os << "Front"; 
				break;

			case eCubePlane::BACK: 
				os << "Back"; 
				break;

			case eCubePlane::TOP: 
				os << "Top"; 
				break;

			case eCubePlane::BOTTOM:
				os << "Bottom"; 
				break;

			case eCubePlane::LEFT:
				os << "Left"; 
				break;

			case eCubePlane::RIGHT: 
				os << "Right"; 
				break;

			default:
				os << "Unknown plane: " << static_cast<std::underlying_type_t<eCubePlane>>(plane);
			}

			return os;
		}
	}
}