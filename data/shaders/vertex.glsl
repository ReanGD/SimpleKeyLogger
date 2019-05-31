#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vTangent;
layout (location = 3) in vec2 vTexCoord;

smooth out vec3 pNormal;
smooth out vec2 pTexCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;

void main() {
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(vPosition, 1.0f);
    pNormal		= uNormalMatrix * vNormal;
	pTexCoord   = vTexCoord;
}
