#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include "engine/common/noncopyable.h"


class Shader;
class Camera;
class Texture;
class Material : Noncopyable {
    struct PrivateArg{};
    friend class MaterialManager;

public:
    Material() = delete;
    Material(const PrivateArg&, uint32_t id, const std::shared_ptr<Shader>& shader,
        const glm::vec3& baseColor, const std::shared_ptr<Texture>& baseTexture, uint baseTextureUnit);
    ~Material() = default;

public:
    uint32_t GetId() const noexcept { return m_id; }

    void Bind(const std::shared_ptr<Camera>& camera, const glm::mat4& matModel, const glm::mat3& matNormal) const;
    void Unbind() const;

private:
    const uint32_t m_id = 0;
    const std::shared_ptr<Shader> m_shader;
    const glm::vec3 m_baseColor = glm::vec3(0);
    const std::shared_ptr<Texture> m_baseTexture;
    const uint m_baseTextureUnit = 0;
};
