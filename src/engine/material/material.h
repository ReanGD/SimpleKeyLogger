#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include "engine/common/math.h"
#include "engine/common/noncopyable.h"


class Shader;
class Camera;
class Texture;
class Material : Noncopyable {
    friend class MaterialManager;
    struct Desc {
        std::shared_ptr<Shader> m_shader = nullptr;
        math::Color3 m_baseColor;
        std::shared_ptr<Texture> m_baseTexture = nullptr;
        uint m_baseTextureUnit = 0;

        // hash function
        std::size_t operator()(const Desc& value) const;
        bool operator==(const Desc& other) const;
    };

public:
    Material() = delete;
    Material(uint32_t id, const Desc& desc);
    ~Material() = default;

public:
    uint32_t GetId() const noexcept { return m_id; }

    void Bind(const std::shared_ptr<Camera>& camera, const glm::mat4& matModel, const glm::mat3& matNormal) const;
    void Unbind() const;

private:
    const uint32_t m_id = 0;
    const Desc m_desc;
};
