#pragma once

#include <memory>
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

    std::shared_ptr<Texture> Create(const ImageHeader& header, std::string& error) noexcept;

    // auto generate mip levels if Need
    std::shared_ptr<Texture> Create(const Image& image, std::string& error) noexcept;
    std::shared_ptr<Texture> Create(const Image& image, bool generateMipLevelsIfNeed, std::string& error) noexcept;

    // auto generate mip levels if Need
    std::shared_ptr<Texture> Load(const std::string& path, std::string& error) noexcept;
    std::shared_ptr<Texture> Load(const std::string& path, bool generateMipLevelsIfNeed, std::string& error) noexcept;
};
