#include "engine/material/material.h"

#include "engine/camera/camera.h"
#include "engine/material/shader.h"
#include "engine/material/texture.h"


Material::Material(const PrivateArg&, uint32_t id, const std::shared_ptr<Shader>& shader,
    const glm::vec3& baseColor, const std::shared_ptr<Texture>& baseTexture, uint baseTextureUnit)
    : m_id(id)
    , m_shader(shader)
    , m_baseColor(baseColor)
    , m_baseTexture(baseTexture)
    , m_baseTextureUnit(baseTextureUnit) {

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
