#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include "engine/common/math.h"
#include "engine/common/noncopyable.h"


class UniformBufferDecl;
class Shader : Noncopyable {
    struct PrivateArg{};
    friend class ShaderManager;

public:
    Shader() = delete;
    Shader(const PrivateArg&, uint32_t id, uint handle);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    uint32_t GetId() const noexcept { return m_id; }
    std::shared_ptr<UniformBufferDecl> GetUBDecl(const char* name);

    void SetBool(const char* name, bool value) const;
    void SetInt(const char* name, int value) const;
    void SetFloat(const char* name, float value) const;
    void SetVec2(const char* name, const glm::vec2& vec) const;
    void SetVec2(const char* name, float x, float y) const;
    void SetVec3(const char* name, const glm::vec3& vec) const;
    void SetVec3(const char* name, float x, float y, float z) const;
    void SetVec4(const char* name, const glm::vec4& vec) const;
    void SetVec4(const char* name, float x, float y, float z, float w) const;
    void SetMat2(const char* name, const glm::mat2& mat) const;
    void SetMat3(const char* name, const glm::mat3& mat) const;
    void SetMat4(const char* name, const glm::mat4& mat) const;
    void SetColor3(const char* name, math::Color3f value) const;
    void SetColor4(const char* name, math::Color4f value) const;

private:
    void Destroy();

private:
    const uint32_t m_id = 0;
    const uint m_handle = 0;
};
