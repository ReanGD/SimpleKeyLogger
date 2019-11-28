#include "engine/material/texture_manager.h"

#include "engine/common/path.h"
#include "engine/common/exception.h"
#include "engine/material/texture.h"
#include "engine/common/hash_combine.h"


std::shared_ptr<Texture> TextureManager::Create(const ImageHeader& header) {
    const bool generateMipLevelsIfNeed = false;
    return std::make_shared<Texture>(++m_lastId, ImageView(header, 0, nullptr), generateMipLevelsIfNeed, Texture::PrivateArg{});
}

std::shared_ptr<Texture> TextureManager::Create(const ImageView& image) {
    return Create(image, true);
}

std::shared_ptr<Texture> TextureManager::Create(const ImageView& image, bool generateMipLevelsIfNeed) {
    return std::make_shared<Texture>(++m_lastId, image, generateMipLevelsIfNeed, Texture::PrivateArg{});
}

std::shared_ptr<Texture> TextureManager::Load(const std::filesystem::path& path) {
    return Load(path, true);
}

std::shared_ptr<Texture> TextureManager::Load(const std::filesystem::path& path, bool generateMipLevelsIfNeed) {
    auto fullPath = path;
    try {
        fullPath = FileManager::Get().GetRealPath(path);
        CacheKey key{fullPath, generateMipLevelsIfNeed};
        if (auto it = m_cache.find(key); it != m_cache.cend()) {
            return it->second;
        }

        Image image(fullPath.c_str(), true);
        auto result = std::make_shared<Texture>(++m_lastId, image.view, generateMipLevelsIfNeed, Texture::PrivateArg{});
        m_cache[key] = result;
        return result;
    } catch(const std::exception& e) {
        throw EngineError("failed to create texture from file '{}', error: {}", fullPath.c_str(), e.what());
    }
}


std::size_t TextureManager::CacheKey::operator()(const TextureManager::CacheKey& value) const {
    std::size_t h = 0;
    hash_combine(h, value.path.string());
    hash_combine(h, static_cast<uint8_t>(value.generateMipLevelsIfNeed));
    return h;
}

bool TextureManager::CacheKey::operator==(const TextureManager::CacheKey& other) const {
    return ((generateMipLevelsIfNeed == other.generateMipLevelsIfNeed) && (path == other.path));
}


DynamicTexture::DynamicTexture(const ImageHeader& header)
    : m_header(header) {

}

std::shared_ptr<Texture> DynamicTexture::GetTexture() {
    if (m_texture == nullptr) {
        m_texture = TextureManager::Get().Create(m_header);
    }

    return m_texture;
}

std::shared_ptr<Texture> DynamicTexture::GetTexture(const ImageHeader& header) {
    if (m_texture == nullptr) {
        m_texture = TextureManager::Get().Create(m_header);
    } else if (m_header != header) {
        m_texture->Update(ImageView(header, 0, nullptr));
    }
    m_header = header;

    return m_texture;
}

std::shared_ptr<Texture> DynamicTexture::UpdateOrCreate(const ImageView& image) {
    return UpdateOrCreate(image, true);
}

std::shared_ptr<Texture> DynamicTexture::UpdateOrCreate(const ImageView& image, bool generateMipLevelsIfNeed) {
    if (m_texture == nullptr) {
        m_texture = TextureManager::Get().Create(image, generateMipLevelsIfNeed);
    } else {
        m_texture->Update(image);
    }
    m_header = image.header;

    return m_texture;
}
