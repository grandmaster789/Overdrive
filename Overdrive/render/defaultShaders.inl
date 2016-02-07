#pragma once

#include "defaultShaders.h"

namespace overdrive {
	namespace render {
		// === attributes::Position ===

		template <>
		static const char* DefaultShader<attributes::Position>::getVertexShader() {
			return R"(
				#version 400

				uniform mat4 uModel;
				uniform mat4 uView;
				uniform mat4 uProjection;

				layout (location = 0) in vec3 aPosition;

				void main() {
					gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
				}
			)";
		}

		template <>
		static const char* DefaultShader<attributes::Position>::getFragmentShader() {
			return R"(
				#version 400

				out vec4 fragColor;

				void main() {
					fragColor = vec4(0.8, 0.8, 1.0, 1.0);
				}
			)";
		}

		// === attributes::PositionColor ===

		template <>
		static const char* DefaultShader<attributes::PositionColor>::getVertexShader() {
			return R"(
				#version 400

				uniform mat4 uModel;
				uniform mat4 uView;
				uniform mat4 uProjection;

				layout (location = 0) in vec3 aPosition;
				layout (location = 3) in vec4 aColor;

				out vec4 vtxColor;

				void main() {
					gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
				}
			)";
		}

		template <>
		static const char* DefaultShader<attributes::PositionColor>::getFragmentShader() {
			return R"(
				#version 400

				in vec4 vtxColor;

				out vec4 fragColor;

				void main() {
					fragColor = vtxColor;
				}
			)";
		}

		// === attributes::PositionTexCoord ===

		template <>
		static const char* DefaultShader<attributes::PositionTexCoord>::getVertexShader() {
			return R"(
				#version 400

				uniform mat4 uModel;
				uniform mat4 uView;
				uniform mat4 uProjection;

				layout (location = 0) in vec3 aPosition;
				layout (location = 2) in vec2 aTexCoord;

				smooth out vec2 vtxTexCoord;

				void main() {
					gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
					vtxTexCoord = aTexCoord;
				}
			)";
		}

		template <>
		static const char* DefaultShader<attributes::PositionTexCoord>::getFragmentShader() {
			return R"(
				#version 400

				uniform sampler2D uTexture;
				smooth in vec2 vtxTexCoord;

				out vec4 fragColor;

				void main() {
					fragColor = texture(uTexture, vtxTexCoord);
				}
			)";
		}

		// === attributes::PositionNormal ===

		template <>
		static const char* DefaultShader<attributes::PositionNormal>::getVertexShader() {
			return R"(
				#version 400

				uniform mat4 uModel;
				uniform mat4 uView;
				uniform mat4 uProjection;

				uniform vec4 uLightDirection;
				uniform vec4 uLightAmbient;
				uniform vec4 uLightDiffuse;

				layout (location = 0) in vec3 aPosition;
				layout (location = 1) in vec3 aNormal;

				out vec4 vtxColor;

				void main() {
					gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
					
					float diffuseFactor = clamp(
						dot(
							-aNormal, 
							normalize(uLightDirection.xyz)
						), 
						0.0, 
						1.0
					);

					vtxColor = uLightAmbient + uLightDiffuse * diffuseFactor;					
				}
			)";
		}

		template <>
		static const char* DefaultShader<attributes::PositionNormal>::getFragmentShader() {
			return R"(
				#version 400

				in vec4 vtxColor;

				out vec4 fragColor;

				void main() {
					fragColor = vtxColor;
				}
			)";
		}

		// === attributes::PositionNormalColor ===

		template <>
		static const char* DefaultShader<attributes::PositionNormalColor>::getVertexShader() {
			return R"(
				#version 400

				uniform mat4 uModel;
				uniform mat4 uView;
				uniform mat4 uProjection;

				uniform vec4 uLightDirection;
				uniform vec4 uLightAmbient;
				uniform vec4 uLightDiffuse;

				layout (location = 0) in vec3 aPosition;
				layout (location = 1) in vec3 aNormal;
				layout (location = 3) in vec4 aColor;

				out vec4 vtxColor;

				void main() {
					gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
					
					float diffuseFactor = clamp(
						dot(
							-aNormal, 
							normalize(uLightDirection.xyz)
						), 
						0.0, 
						1.0
					);

					vec4 lightColor = uLightAmbient + uLightDiffuse * diffuseFactor;
					
					vtxColor = aColor * lightColor;
				}
			)";
		}

		template <>
		static const char* DefaultShader<attributes::PositionNormalColor>::getFragmentShader() {
			return R"(
				#version 400

				in vec4 vtxColor;				

				out vec4 fragColor;

				void main() {
					fragColor = vtxColor;
				}
			)";
		}

		// === attributes::PositionNormalTexCoord ===

		template <>
		static const char* DefaultShader<attributes::PositionNormalTexCoord>::getVertexShader() {
			return R"(
				#version 400

				uniform mat4 uModel;
				uniform mat4 uView;
				uniform mat4 uProjection;

				uniform vec4 uLightDirection;
				uniform vec4 uLightAmbient;
				uniform vec4 uLightDiffuse;

				layout (location = 0) in vec3 aPosition;
				layout (location = 1) in vec3 aNormal;
				layout (location = 2) in vec2 aTexCoord;

				out vec4 vtxColor;
				smooth out vec2 vtxTexCoord;

				void main() {
					gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
					
					float diffuseFactor = clamp(
						dot(
							-aNormal, 
							normalize(uLightDirection.xyz)
						), 
						0.0, 
						1.0
					);

					vtxColor = uLightAmbient + uLightDiffuse * diffuseFactor;
					vtxTexCoord = aTexCoord;
				}
			)";
		}

		template <>
		static const char* DefaultShader<attributes::PositionNormalTexCoord>::getFragmentShader() {
			return R"(
				#version 400

				uniform sampler2D uTexture;

				in vec4 vtxColor;
				smooth in vec2 vtxTexCoord;

				out vec4 fragColor;

				void main() {
					vec4 texColor = texture(uTexture, vtxTexCoord);

					fragColor = vtxColor * texColor;
				}
			)";
		}

		// === attributes::PositionNormalTexCoordColor ===

		template <>
		static const char* DefaultShader<attributes::PositionNormalTexCoordColor>::getVertexShader() {
			return R"(
				#version 400

				uniform mat4 uModel;
				uniform mat4 uView;
				uniform mat4 uProjection;

				uniform vec4 uLightDirection;
				uniform vec4 uLightAmbient;
				uniform vec4 uLightDiffuse;

				layout (location = 0) in vec3 aPosition;
				layout (location = 1) in vec3 aNormal;
				layout (location = 2) in vec2 aTexCoord;
				layout (location = 3) in vec4 aColor;

				out vec4 vtxColor;
				smooth out vec2 vtxTexCoord;

				void main() {
					gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
					
					float diffuseFactor = clamp(
						dot(
							-aNormal, 
							normalize(uLightDirection.xyz)
						), 
						0.0, 
						1.0
					);

					vtxColor = aColor * (uLightAmbient + uLightDiffuse * diffuseFactor);
					vtxTexCoord = aTexCoord;
				}
			)";
		}

		template <>
		static const char* DefaultShader<attributes::PositionNormalTexCoordColor>::getFragmentShader() {
			return R"(
				#version 400

				uniform sampler2D uTexture;

				in vec4 vtxColor;
				smooth in vec2 vtxTexCoord;

				out vec4 fragColor;

				void main() {
					vec4 texColor = texture(uTexture, vtxTexCoord);

					fragColor = vtxColor * texColor;
				}
			)";
		}
	}
}