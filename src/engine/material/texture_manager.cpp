#include "engine/material/texture_manager.h"

#include "engine/common/exception.h"


void TextureManager::AddFindPath(const std::filesystem::path& path) {
    m_basePaths.push_back(path);
}

std::filesystem::path TextureManager::GetFullPath(const std::filesystem::path& path) const {
    if (std::filesystem::exists(path)) {
        return path;
    }

    std::filesystem::path result;
    for (const auto& base: m_basePaths) {
        result = base / path;
        if (std::filesystem::exists(result)) {
            return result;
        }
    }

    throw EngineError("the texture of the path '{}' was not found", path.c_str());
}

bool TextureManager::GetFullPath(const std::filesystem::path& inPath, std::filesystem::path& outPath, std::string& error) const noexcept {
    try {
        outPath = GetFullPath(inPath);
        return true;
    } catch(const std::exception& e) {
        error = e.what();
        return false;
    }
}

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
        fullPath = GetFullPath(path);
        Image image(fullPath.c_str(), true);
        return std::make_shared<Texture>(image.view, generateMipLevelsIfNeed, Texture::PrivateArg{});
    } catch(const std::exception& e) {
        throw EngineError("failed to create texture from file '{}', error: {}", fullPath.c_str(), e.what());
    }
}
