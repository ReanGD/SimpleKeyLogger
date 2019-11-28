#pragma once

#include <vector>
#include <filesystem>
#include <unordered_map>

#include "engine/material/image.h"
#include "engine/common/noncopyable.h"


class Texture;
class TextureManager : Noncopyable {
private:
    TextureManager() = default;
    ~TextureManager() = default;

public:
    static TextureManager& Get() noexcept {
        static TextureManager instance;
        return instance;
    }

    std::shared_ptr<Texture> Create(const ImageHeader& header);

    // auto generate mip levels if Need
    std::shared_ptr<Texture> Create(const ImageView& image);
    std::shared_ptr<Texture> Create(const ImageView& image, bool generateMipLevelsIfNeed);

    // auto generate mip levels if Need
    std::shared_ptr<Texture> Load(const std::filesystem::path& path);
    std::shared_ptr<Texture> Load(const std::filesystem::path& path, bool generateMipLevelsIfNeed);

private:
    struct CacheKey {
        std::filesystem::path path;
        bool generateMipLevelsIfNeed;

        // hash function
        std::size_t operator()(const CacheKey& value) const;
        bool operator==(const CacheKey& other) const;
    };

    std::unordered_map<CacheKey, std::shared_ptr<Texture>, CacheKey> m_cache;
    uint32_t m_lastId = 0;
};

class DynamicTexture : Noncopyable {
public:
    DynamicTexture() = default;
    DynamicTexture(const ImageHeader& header);
    ~DynamicTexture() = default;

    std::shared_ptr<Texture> GetTexture();
    std::shared_ptr<Texture> GetTexture(const ImageHeader& header);

    // auto generate mip levels if Need
    std::shared_ptr<Texture> UpdateOrCreate(const ImageView& image);
    std::shared_ptr<Texture> UpdateOrCreate(const ImageView& image, bool generateMipLevelsIfNeed);
private:
    ImageHeader m_header;
    std::shared_ptr<Texture> m_texture = nullptr;
};
