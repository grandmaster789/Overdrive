#ifndef OVERDRIVE_MATH_AABB_H
#define OVERDRIVE_MATH_AABB_H

#include "opengl.h"
#include <algorithm>
#include <stdexcept>

namespace overdrive {
	namespace math {
		// Axis-Aligned Bounding Box
		class AABB {
		public:
			void clear();

			glm::vec3 getCorner(unsigned char index) const;

			void transform(glm::mat4 m);

			bool merge(const AABB& box); // returns false if the other box was already full contained within this one

		private:
			glm::vec3 mMinCorner = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 mMaxCorner = glm::vec3(0.0f, 0.0f, 0.0f);
		};
	}
}

#endif
