#version 420 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outNormal;

out gl_PerVertex {
	vec4 gl_Position;	
};

void main() {
	outPosition = inPosition;	// passthrough worldspace position
	outNormal = inNormal;		// passthrough worldspace normal
	
	// calculate cameraspace position
	gl_Position = 
		projectionMatrix * 
		viewMatrix * 
		modelMatrix * 
		vec4(inPosition, 1.0f);
}