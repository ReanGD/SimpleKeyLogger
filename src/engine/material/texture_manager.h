#pragma once

#include <vector>
#include <filesystem>
#include "engine/material/texture.h"


class TextureManager : Noncopyable {
private:
    TextureManager() = default;
    ~TextureManager() = default;

public:
    static TextureManager& Get() noexcept {
        static TextureManager instance;
        return instance;
    }

    void AddFindPath(const std::filesystem::path& path);

    std::shared_ptr<Texture> Create(const ImageHeader& header, std::string& error) noexcept;

    // auto generate mip levels if Need
    std::shared_ptr<Texture> Create(const Image& image, std::string& error) noexcept;
    std::shared_ptr<Texture> Create(const Image& image, bool generateMipLevelsIfNeed, std::string& error) noexcept;

    // auto generate mip levels if Need
    std::shared_ptr<Texture> Load(const std::filesystem::path& path, std::string& error) noexcept;
    std::shared_ptr<Texture> Load(const std::filesystem::path& path, bool generateMipLevelsIfNeed, std::string& error) noexcept;

private:
    std::vector<std::filesystem::path> m_basePaths;
};
