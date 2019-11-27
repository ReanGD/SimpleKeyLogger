#include "engine/material/material_manager.h"

#include "engine/material/material.h"
#include "engine/material/texture_manager.h"


MaterialManager::Builder::Builder(const std::shared_ptr<Shader>& shader)
    : m_shader(shader) {

}

MaterialManager::Builder& MaterialManager::Builder::BaseColor(const glm::vec3& color) noexcept {
    m_baseColor = color;

    return *this;
}

MaterialManager::Builder& MaterialManager::Builder::BaseTexture(uint unit, const std::shared_ptr<Texture>& texture) noexcept {
    m_baseTextureUnit = unit;
    m_baseTexture = texture;

    return *this;
}

MaterialManager::Builder& MaterialManager::Builder::BaseTexture(uint unit, const std::filesystem::path& path) {
    m_baseTextureUnit = unit;
    m_baseTexture = TextureManager::Get().Load(path);
    return *this;
}

MaterialManager::Builder& MaterialManager::Builder::BaseTexture(uint unit, const std::filesystem::path& path, bool generateMipLevelsIfNeed) {
    m_baseTextureUnit = unit;
    m_baseTexture = TextureManager::Get().Load(path, generateMipLevelsIfNeed);

    return *this;
}

std::shared_ptr<Material> MaterialManager::Builder::Build() {
    return MaterialManager::Get().Build(*this);
}

std::shared_ptr<Material> MaterialManager::Build(MaterialManager::Builder& builder) {
    return std::make_shared<Material>(Material::PrivateArg{}, ++m_lastId, builder.m_shader,
        builder.m_baseColor, builder.m_baseTexture, builder.m_baseTextureUnit);
}
