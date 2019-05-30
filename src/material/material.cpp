#include "material/material.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

Material::Material(const Shader& shader)
    : m_shader(shader) {

}

Material& Material::SetShader(const Shader& shader) noexcept {
    m_shader = shader;

    return *this;
}

Material& Material::SetBaseColor(const glm::vec3& color) noexcept {
    m_baseColor = color;

    return *this;
}

Material& Material::SetBaseTexture(uint unit, const Texture& texture) noexcept {
    m_baseTexture = texture;
    m_baseTextureUnit = unit;

    return *this;
}

void Material::Bind(const std::shared_ptr<Camera>& camera, const glm::mat4& matWorld) const {
    m_shader.Bind();
    m_baseTexture.Bind(m_baseTextureUnit);
    m_shader.SetInt("uBaseTexture", int(m_baseTextureUnit));
    m_shader.SetVec3("uBaseColor", m_baseColor);

    m_shader.SetMat4("uProjMatrix", camera->GetProjMatrix());
    m_shader.SetMat4("uViewMatrix", camera->GetViewMatrix());
    m_shader.SetVec3("uToEyeDirection", camera->GetToEyeDirection());

    m_shader.SetMat4("uModelMatrix", matWorld);
    glm::mat3 matNorm = glm::inverseTranspose(glm::mat3(matWorld));
    m_shader.SetMat3("uNormalMatrix", matNorm);
}

void Material::Unbind() const {
    m_baseTexture.Unbind(m_baseTextureUnit);
    m_shader.Unbind();
}
