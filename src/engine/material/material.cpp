#include "engine/material/material.h"

#include "engine/camera/camera.h"
#include "engine/material/shader.h"
#include "engine/material/texture.h"
#include "engine/common/hash_combine.h"


std::size_t Material::Desc::operator()(const Material::Desc& value) const {
    std::size_t h = 0;
    hash_combine(h, value.m_shader->GetId());
    hash_combine(h, value.m_baseColor.value);
    if (value.m_baseTexture) {
        hash_combine(h, value.m_baseTexture->GetId());
        hash_combine(h, value.m_baseTextureUnit);
    }

    return h;
}

bool Material::Desc::operator==(const Material::Desc& other) const {
    return ((m_baseTextureUnit == other.m_baseTextureUnit) &&
        (m_shader->GetId() == other.m_shader->GetId()) &&
        (m_baseTexture->GetId() == other.m_baseTexture->GetId()) &&
        (m_baseColor.value == other.m_baseColor.value)
        );
}

Material::Material(uint32_t id, const Material::Desc& desc)
    : m_id(id)
    , m_desc(desc) {
}

uint32_t Material::GetShaderId() const noexcept {
    return m_desc.m_shader->GetId();
}

void Material::BindShader() const {
    m_desc.m_shader->Bind();
}

void Material::BindUniforms(const std::shared_ptr<Camera>& camera, const glm::mat4& matModel, const glm::mat3& matNormal) const {
    if (m_desc.m_baseTexture) {
        m_desc.m_baseTexture->Bind(m_desc.m_baseTextureUnit);
        m_desc.m_shader->SetInt("uBaseTexture", int(m_desc.m_baseTextureUnit));
    }
    m_desc.m_shader->SetColor3("uBaseColor", m_desc.m_baseColor.ToColor3f());

    m_desc.m_shader->SetMat4("uProjMatrix", camera->GetProjMatrix());
    m_desc.m_shader->SetMat4("uViewMatrix", camera->GetViewMatrix());
    m_desc.m_shader->SetVec3("uToEyeDirection", camera->GetToEyeDirection());

    m_desc.m_shader->SetMat4("uModelMatrix", matModel);
    m_desc.m_shader->SetMat3("uNormalMatrix", matNormal);
}

void Material::Unbind() const {
    if (m_desc.m_baseTexture) {
        m_desc.m_baseTexture->Unbind(m_desc.m_baseTextureUnit);
    }
    m_desc.m_shader->Unbind();
}
