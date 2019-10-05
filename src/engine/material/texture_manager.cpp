#include "engine/material/texture_manager.h"

#include <fmt/format.h>
#include "engine/material/image_loader.h"

void TextureManager::AddFindPath(const std::filesystem::path& path) {
    m_basePaths.push_back(path);
}

bool TextureManager::GetFullPath(const std::filesystem::path& inPath, std::filesystem::path& outPath, std::string& error) const noexcept {
    outPath = inPath;
    if (std::filesystem::exists(outPath)) {
        return true;
    }

    for (const auto& base: m_basePaths) {
        outPath = base / inPath;
        if (std::filesystem::exists(outPath)) {
            return true;
        }
    }

    error = fmt::format("the texture of the path '{}' was not found", inPath.c_str());
    return false;
}

std::shared_ptr<Texture> TextureManager::Create(const ImageHeader& header, std::string& error) noexcept {
    Texture::Result isSuccess;
    const bool generateMipLevelsIfNeed = false;
    auto texture = std::make_shared<Texture>(Image(header, 0, nullptr), generateMipLevelsIfNeed, error, isSuccess);

    return isSuccess.value ? texture : nullptr;
}

std::shared_ptr<Texture> TextureManager::Create(const Image& image, std::string& error) noexcept {
    return Create(image, true, error);
}

std::shared_ptr<Texture> TextureManager::Create(const Image& image, bool generateMipLevelsIfNeed, std::string& error) noexcept {
    Texture::Result isSuccess;
    auto texture = std::make_shared<Texture>(image, generateMipLevelsIfNeed, error, isSuccess);

    return isSuccess.value ? texture : nullptr;
}

std::shared_ptr<Texture> TextureManager::Load(const std::filesystem::path& path, std::string& error) noexcept {
    return Load(path, true, error);
}

std::shared_ptr<Texture> TextureManager::Load(const std::filesystem::path& path, bool generateMipLevelsIfNeed, std::string& error) noexcept {
    std::filesystem::path fullPath;
    if (!GetFullPath(path, fullPath, error)) {
        return nullptr;
    }

    Image image;
    if(!image_loader::Load(fullPath.c_str(), image, error)) {
        return nullptr;
    }

    Texture::Result isSuccess;
    auto texture = std::make_shared<Texture>(image, generateMipLevelsIfNeed, error, isSuccess);
    image_loader::Free(image);
    if (!isSuccess.value) {
        error = fmt::format("failed to create texture from file '{}', error: {}", fullPath.c_str(), error);
    }

    return isSuccess.value ? texture : nullptr;
}
