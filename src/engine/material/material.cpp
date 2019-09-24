#include "engine/material/material.h"


Material::Material(const std::shared_ptr<Shader>& shader)
    : m_shader(shader) {

}

Material& Material::SetShader(const std::shared_ptr<Shader>& shader) noexcept {
    m_shader = shader;

    return *this;
}

Material& Material::SetBaseColor(const glm::vec3& color) noexcept {
    m_baseColor = color;

    return *this;
}

Material& Material::SetBaseTexture(uint unit, const std::shared_ptr<Texture>& texture) noexcept {
    m_baseTexture = texture;
    m_baseTextureUnit = unit;

    return *this;
}

void Material::Bind(const std::shared_ptr<Camera>& camera, const glm::mat4& matModel, const glm::mat3& matNormal) const {
    m_shader->Bind();
    if (m_baseTexture) {
        m_baseTexture->Bind(m_baseTextureUnit);
        m_shader->SetInt("uBaseTexture", int(m_baseTextureUnit));
    }
    m_shader->SetVec3("uBaseColor", m_baseColor);

    m_shader->SetMat4("uProjMatrix", camera->GetProjMatrix());
    m_shader->SetMat4("uViewMatrix", camera->GetViewMatrix());
    m_shader->SetVec3("uToEyeDirection", camera->GetToEyeDirection());

    m_shader->SetMat4("uModelMatrix", matModel);
    m_shader->SetMat3("uNormalMatrix", matNormal);
}

void Material::Unbind() const {
    if (m_baseTexture) {
        m_baseTexture->Unbind(m_baseTextureUnit);
    }
    m_shader->Unbind();
}
