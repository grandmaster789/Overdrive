#pragma once

#include "../opengl.h"
#include <boost/fusion/adapted.hpp>

namespace overdrive {
	namespace render {
		// some common vertex attribute formats
		namespace attributes {
			struct Position {
				glm::vec3 mPosition;
			};

			struct PositionColor {
				glm::vec3 mPosition;
				glm::vec4 mColor;
			};

			struct PositionTexCoord {
				glm::vec3 mPosition;
				glm::vec2 mTexCoord;
			};

			struct PositionNormalColor {
				glm::vec3 mPosition;
				glm::vec3 mNormal;
				glm::vec4 mColor;
			};

			struct PositionNormalTexCoord {
				glm::vec3 mPosition;
				glm::vec3 mNormal;
				glm::vec2 mTexCoord;
			};

			struct PositionNormalTexCoordColor {
				glm::vec3 mPosition;
				glm::vec3 mNormal;
				glm::vec2 mTexCoord;
				glm::vec4 mColor;
			};
		}
	}
}

BOOST_FUSION_ADAPT_STRUCT(
	overdrive::render::attributes::Position,
	(glm::vec3, mPosition)
)

BOOST_FUSION_ADAPT_STRUCT(
	overdrive::render::attributes::PositionColor,
	(glm::vec3, mPosition)
	(glm::vec4, mColor)
)

BOOST_FUSION_ADAPT_STRUCT(
	overdrive::render::attributes::PositionTexCoord,
	(glm::vec3, mPosition)
	(glm::vec2, mTexCoord)
)

BOOST_FUSION_ADAPT_STRUCT(
	overdrive::render::attributes::PositionNormalColor,
	(glm::vec3, mPosition)
	(glm::vec3, mNormal)
	(glm::vec4, mColor)
)

BOOST_FUSION_ADAPT_STRUCT(
	overdrive::render::attributes::PositionNormalTexCoord,
	(glm::vec3, mPosition)
	(glm::vec3, mNormal)
	(glm::vec2, mTexCoord)
)

BOOST_FUSION_ADAPT_STRUCT(
	overdrive::render::attributes::PositionNormalTexCoordColor,
	(glm::vec3, mPosition)
	(glm::vec3, mNormal)
	(glm::vec2, mTexCoord)
	(glm::vec4, mColor)
)
