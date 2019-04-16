#version 330 core

smooth in vec3 pNormal;
smooth in vec2 pTexCoord;

out vec4 color;

uniform sampler2D ourTexture1;

/*
    N - нормаль
    L - обратное направление света (RLightDir)

    Все вектора нормализованы
*/
float lambert(vec3 N, vec3 L) {
    float cosNL = dot(N, L);
    return max(cosNL, 0.0);
}

void main() {
    vec3 Normal = normalize(pNormal);
    vec3 RLightDir = normalize(vec3(0, 1, 0));
    vec3 Facture = vec3(0.5, 0.5, 0.5);

    color = vec4(Facture * lambert(Normal, RLightDir), 1.0);
}
