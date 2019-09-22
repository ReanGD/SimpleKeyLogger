#include "engine/material/texture_manager.h"

#include <filesystem>
#include <fmt/format.h>
#include "engine/material/image_loader.h"


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

std::shared_ptr<Texture> TextureManager::Load(const std::string& path, std::string& error) noexcept {
    return Load(path, true, error);
}

std::shared_ptr<Texture> TextureManager::Load(const std::string& path, bool generateMipLevelsIfNeed, std::string& error) noexcept {
    const auto fullPath = std::filesystem::current_path() / "assets" / "textures" / path;

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
