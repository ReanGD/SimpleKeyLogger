#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matWorld;

void main() {
	vec4 posWorld = matWorld * vec4(position, 1.0f);
	mat4 matViewProj = matProj * matView;
	gl_Position = matViewProj * posWorld;
	ourColor = color;
	TexCoord = vec2(texCoord.x, texCoord.y);
}
