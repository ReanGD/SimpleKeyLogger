#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include "material/shader.h"
#include "material/texture.h"
#include "camera/camera.h"


class Material {
public:
    Material() = delete;
    Material(const Shader& shader);
    ~Material() = default;

public:
    Material& SetShader(const Shader& shader) noexcept;
    Material& SetBaseColor(const glm::vec3& color) noexcept;
    Material& SetBaseTexture(uint unit, const Texture& texture) noexcept;

    void Bind(const std::shared_ptr<Camera>& camera, const glm::mat4& matWorld) const;
    void Unbind() const;

private:
    Shader m_shader;
    glm::vec3 m_baseColor = glm::vec3(0);
    Texture m_baseTexture;
    uint m_baseTextureUnit = 0;
};