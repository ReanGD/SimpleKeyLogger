#include "engine/material/material_manager.h"

#include "engine/material/material.h"
#include "engine/material/texture_manager.h"


MaterialManager::Builder::Builder(const std::shared_ptr<Shader>& shader) {
    m_desc.m_shader = shader;
}

MaterialManager::Builder& MaterialManager::Builder::BaseColor(math::Color3 color) noexcept {
    m_desc.m_baseColor = color;

    return *this;
}

MaterialManager::Builder& MaterialManager::Builder::BaseTexture(uint unit, const std::shared_ptr<Texture>& texture) noexcept {
    m_desc.m_baseTextureUnit = unit;
    m_desc.m_baseTexture = texture;

    return *this;
}

MaterialManager::Builder& MaterialManager::Builder::BaseTexture(uint unit, const std::filesystem::path& path, bool generateMipLevelsIfNeed) {
    m_desc.m_baseTextureUnit = unit;
    m_desc.m_baseTexture = TextureManager::Get().Load(path, generateMipLevelsIfNeed);

    return *this;
}

std::shared_ptr<Material> MaterialManager::Builder::Build() {
    return MaterialManager::Get().Build(m_desc);
}

std::shared_ptr<Material> MaterialManager::Build(const Material::Desc& desc) {
    if (auto it = m_cache.find(desc); it != m_cache.cend()) {
        return it->second;
    }

    auto result = std::make_shared<Material>(++m_lastId, desc);
    m_cache[desc] = result;

    return result;
}
