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
    std::filesystem::path GetFullPath(const std::filesystem::path& path) const;
    bool GetFullPath(const std::filesystem::path& inPath, std::filesystem::path& outPath, std::string& error) const noexcept;

    std::shared_ptr<Texture> Create(const ImageHeader& header);

    // auto generate mip levels if Need
    std::shared_ptr<Texture> Create(const ImageView& image);
    std::shared_ptr<Texture> Create(const ImageView& image, bool generateMipLevelsIfNeed);

    // auto generate mip levels if Need
    std::shared_ptr<Texture> Load(const std::filesystem::path& path);
    std::shared_ptr<Texture> Load(const std::filesystem::path& path, bool generateMipLevelsIfNeed);

private:
    std::vector<std::filesystem::path> m_basePaths;
};
