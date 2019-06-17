#version 330 core

smooth in vec3 pNormal;
smooth in vec2 pTexCoord;

out vec4 color;

uniform vec3 uBaseColor;
uniform vec3 uToEyeDirection;

const vec3 uToLightDirection = vec3(0, 1, 0);
// rgb - цвет источника света, a - ambient интенсивность
const vec4 uLightColor = vec4(1, 0, 1, 0.1);
// uShininess > 0
const float uShininess = 20.0;

/*
	Расчитать освещение по модели Фонга
	Все вектора должны передаваться нормализоваными

	N - нормаль
	L - вектор направленный к источнику света (обратный направлению луча) (uToLightDirection)
	V - вектор обратный направлению взгляда (uToEyeDirection)
	facture  - цвет текстуры или материала
*/
vec3 PhongLighting(vec3 N, vec3 L, vec3 V, vec3 facture) {
	float cosNL = dot(N, L);
	vec3 reflection = 2.0f * N * cosNL - L;
	float cosRV = max(0, dot(reflection, V));
	float powSpecular = pow(cosRV, uShininess);

	// Ambient = facture * uLightColor.a * uLightColor.rgb
	// Diffuse = facture * cosNL * uLightColor.rgb
	// Specular = powSpecular * uLightColor.rgb
	vec3 color = (facture * (uLightColor.a + cosNL) + powSpecular) * uLightColor.rgb;

	return min(color, vec3(1));
}

void main() {
    vec3 normal = normalize(pNormal);
    vec3 facture = uBaseColor;

    vec3 clr = PhongLighting(normal, uToLightDirection, uToEyeDirection, facture);
    color = vec4(clr, 1.0);
}
