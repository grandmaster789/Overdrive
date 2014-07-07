#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 LightPosition;	// light position (in eye coords)
uniform vec3 LightDiffuse; 
uniform vec3 MaterialDiffuse;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjection;

void main() {
    vec3 tnorm = normalize(NormalMatrix * VertexNormal);

    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition, 1.0);

    vec3 s = normalize(vec3(LightPosition - eyeCoords));

    LightIntensity = LightDiffuse * MaterialDiffuse * max(dot(s, tnorm), 0.0);

    gl_Position = ModelViewProjection * vec4(VertexPosition, 1.0);
}
