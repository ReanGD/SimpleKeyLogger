#pragma once

#include <glm/mat4x4.hpp>

#include "engine/material/shader.h"
#include "engine/material/texture.h"
#include "engine/camera/camera.h"


class Material {
public:
    Material() = delete;
    Material(const std::shared_ptr<Shader>& shader);
    ~Material() = default;

public:
    Material& SetShader(const std::shared_ptr<Shader>& shader) noexcept;
    Material& SetBaseColor(const glm::vec3& color) noexcept;
    Material& SetBaseTexture(uint unit, const std::shared_ptr<Texture>& texture) noexcept;

    void Bind(const std::shared_ptr<Camera>& camera, const glm::mat4& matModel, const glm::mat3& matNormal) const;
    void Unbind() const;

private:
    std::shared_ptr<Shader> m_shader;
    glm::vec3 m_baseColor = glm::vec3(0);
    std::shared_ptr<Texture> m_baseTexture;
    uint m_baseTextureUnit = 0;
};
