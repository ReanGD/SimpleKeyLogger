#pragma once

#include <memory>
#include <glm/vec3.hpp>
#include "engine/common/noncopyable.h"


class Shader;
class Texture;
class Material;
class MaterialManager : Noncopyable {
private:
    MaterialManager() = default;
    ~MaterialManager() = default;

public:
    struct Builder {
        friend class MaterialManager;

        Builder() = delete;
        Builder(const std::shared_ptr<Shader>& shader);
        ~Builder() = default;

        Builder& BaseColor(const glm::vec3& color) noexcept;
        Builder& BaseTexture(uint unit, const std::shared_ptr<Texture>& texture) noexcept;
        std::shared_ptr<Material> Build();

    private:
        std::shared_ptr<Shader> m_shader;
        glm::vec3 m_baseColor = glm::vec3(0);
        std::shared_ptr<Texture> m_baseTexture;
        uint m_baseTextureUnit = 0;
    };

    static MaterialManager& Get() noexcept {
        static MaterialManager instance;
        return instance;
    }

private:
    std::shared_ptr<Material> Build(Builder& builder);
};
