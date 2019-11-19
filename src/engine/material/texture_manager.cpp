#include "engine/material/texture_manager.h"

#include "engine/common/path.h"
#include "engine/common/exception.h"


std::shared_ptr<Texture> TextureManager::Create(const ImageHeader& header) {
    const bool generateMipLevelsIfNeed = false;
    return std::make_shared<Texture>(ImageView(header, 0, nullptr), generateMipLevelsIfNeed, Texture::PrivateArg{});
}

std::shared_ptr<Texture> TextureManager::Create(const ImageView& image) {
    return Create(image, true);
}

std::shared_ptr<Texture> TextureManager::Create(const ImageView& image, bool generateMipLevelsIfNeed) {
    return std::make_shared<Texture>(image, generateMipLevelsIfNeed, Texture::PrivateArg{});
}

std::shared_ptr<Texture> TextureManager::Load(const std::filesystem::path& path) {
    return Load(path, true);
}

std::shared_ptr<Texture> TextureManager::Load(const std::filesystem::path& path, bool generateMipLevelsIfNeed) {
    auto fullPath = path;
    try {
        fullPath = FileManager::Get().GetRealPath(path);
        Image image(fullPath.c_str(), true);
        return std::make_shared<Texture>(image.view, generateMipLevelsIfNeed, Texture::PrivateArg{});
    } catch(const std::exception& e) {
        throw EngineError("failed to create texture from file '{}', error: {}", fullPath.c_str(), e.what());
    }
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
    if ((m_texture == nullptr) || (m_header != header)) {
        m_header = header;
        m_texture = TextureManager::Get().Create(m_header);
    }

    return m_texture;
}

std::shared_ptr<Texture> DynamicTexture::GetTexture(uint32_t width, uint32_t height) {
    if ((m_texture == nullptr) || (m_header.width != width) || (m_header.height != height)) {
        m_header.width = width;
        m_header.height = height;
        m_texture = TextureManager::Get().Create(m_header);
    }

    return m_texture;
}

std::shared_ptr<Texture> DynamicTexture::UpdateOrCreate(const ImageView& image) {
    return UpdateOrCreate(image, true);
}

std::shared_ptr<Texture> DynamicTexture::UpdateOrCreate(const ImageView& image, bool generateMipLevelsIfNeed) {
    if ((m_texture == nullptr) || (m_header != image.header)) {
        m_header = image.header;
        m_texture = TextureManager::Get().Create(image, generateMipLevelsIfNeed);
    } else {
        m_texture->Update(image);
    }

    return m_texture;
}
