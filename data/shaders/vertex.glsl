#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vTangent;
layout (location = 3) in vec2 vTexCoord;

out vec2 pTexCoord;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uWorld;

void main() {
	gl_Position = uProj * uView * uWorld * vec4(vPosition, 1.0f);
	pTexCoord = vTexCoord;
}
