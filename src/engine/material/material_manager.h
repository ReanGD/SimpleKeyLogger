#pragma once

#include <filesystem>
#include <unordered_map>
#include "engine/material/material.h"


class MaterialManager : Noncopyable {
private:
    MaterialManager() = default;
    ~MaterialManager() = default;

public:
    struct Builder {
        Builder() = delete;
        Builder(const std::shared_ptr<Shader>& shader);
        ~Builder() = default;

        Builder& BaseColor(math::Color3 color) noexcept;
        Builder& BaseTexture(uint unit, const std::shared_ptr<Texture>& texture) noexcept;
        Builder& BaseTexture(uint unit, const std::filesystem::path& path, bool generateMipLevelsIfNeed = true);

        std::shared_ptr<Material> Build();

    private:
        Material::Desc m_desc;
    };

    static MaterialManager& Get() noexcept {
        static MaterialManager instance;
        return instance;
    }

private:
    std::shared_ptr<Material> Build(const Material::Desc& desc);

    std::unordered_map<Material::Desc, std::shared_ptr<Material>, Material::Desc> m_cache;
    uint32_t m_lastId = 0;
};
