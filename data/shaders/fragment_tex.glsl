#version 330 core

smooth in vec3 pNormal;
smooth in vec2 pTexCoord;

out vec4 color;

uniform sampler2D uBaseTexture;

void main() {
    color = texture(uBaseTexture, pTexCoord);
}
