#include "math/aabb.h"

namespace overdrive {
	namespace math {
		inline void AABB::clear() {
			mMinCorner = glm::vec3(0.0f, 0.0f, 0.0f);
			mMaxCorner = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		inline glm::vec3 AABB::getCorner(unsigned char index) const {
			switch (index) {
			case 0: return glm::vec3(mMinCorner.x, mMinCorner.y, mMaxCorner.z);
			case 1: return glm::vec3(mMaxCorner.x, mMinCorner.y, mMaxCorner.z);
			case 2: return glm::vec3(mMinCorner.x, mMaxCorner.y, mMaxCorner.z);
			case 3: return mMaxCorner;

			case 4: return mMinCorner;
			case 5: return glm::vec3(mMaxCorner.x, mMinCorner.y, mMinCorner.z);
			case 6: return glm::vec3(mMaxCorner.x, mMaxCorner.y, mMinCorner.z);
			case 7: return glm::vec3(mMinCorner.x, mMaxCorner.y, mMinCorner.z);

			default:
				throw std::runtime_error("invalid index");
			}
		}

		inline void AABB::transform(glm::mat4 m) {
			float minA[3] = { mMinCorner.x, mMinCorner.y, mMinCorner.z };
			float minB[3];
			float maxA[3] = { mMaxCorner.x, mMaxCorner.y, mMaxCorner.z };
			float maxB[3];

			for (int i = 0; i < 3; ++i) {
				minB[i] = m[3][i];
				maxB[i] = m[3][i];

				for (int j = 0; j < 3; ++j) {
					float x = minA[j] * m[j][i];
					float y = maxA[j] * m[j][i];
					minB[i] += std::min(x, y);
					maxB[i] += std::max(x, y);
				}
			}

			mMinCorner = glm::vec3(minB[0], minB[1], minB[2]);
			mMaxCorner = glm::vec3(maxB[0], maxB[1], maxB[2]);
		}

		inline bool AABB::merge(const AABB& box) {
			bool result = false;

			if (mMinCorner == mMaxCorner) {
				result = true;

				mMinCorner = box.mMinCorner;
				mMaxCorner = box.mMaxCorner;
			}
			else if (box.mMinCorner != box.mMaxCorner) {
				glm::vec3 minCorner = mMinCorner;
				glm::vec3 maxCorner = mMaxCorner;

				if (box.mMinCorner.x < mMinCorner.x)
					mMinCorner.x = box.mMinCorner.x;

				if (box.mMinCorner.y < mMinCorner.y)
					mMinCorner.y = box.mMinCorner.y;

				if (box.mMinCorner.z < mMinCorner.z)
					mMinCorner.z = box.mMinCorner.z;

				if (box.mMaxCorner.x > mMaxCorner.x)
					mMaxCorner.x = box.mMaxCorner.x;

				if (box.mMaxCorner.y > mMaxCorner.y)
					mMaxCorner.y = box.mMaxCorner.y;

				if (box.mMaxCorner.z > mMaxCorner.z)
					mMaxCorner.z = box.mMaxCorner.z;

				if (mMinCorner != minCorner)
					result = true;

				if (mMaxCorner != maxCorner)
					result = true;
			}

			return result;
		}
	}
}