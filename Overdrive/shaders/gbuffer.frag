#version 420 core

layout (location = 0) in vec3 interpolatedPosition;
layout (location = 1) in vec3 interpolatedNormal;

layout (location = 0) out vec4 outPositionBuffer;
layout (location = 1) out vec3 outNormalBuffer;

void main() {
	outPositionBuffer = vec4(interpolatedPosition, 1.0f);
	outNormalBuffer = normalize(interpolatedNormal);
}