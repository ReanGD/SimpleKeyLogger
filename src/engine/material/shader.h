#pragma once

#include <string>
#include <memory>
#include <glm/mat4x4.hpp>
#include "engine/material/uniform_buffer.h"


class Shader : Noncopyable {
    struct privateArg{};

public:
    Shader() = delete;
    Shader(const privateArg&, uint handle);
    ~Shader();
    static std::shared_ptr<Shader> Create(
        const std::string& vertexShaderName,
        const std::string& fragmentShaderName,
        std::string& error);
    static std::shared_ptr<Shader> Create(
        const std::string& geometryShaderName,
        const std::string& vertexShaderName,
        const std::string& fragmentShaderName,
        std::string& error);

    // TODO: remove it
    uint GetHandle() const noexcept {
        return m_handle;
    }

    void Bind() const;
    void Unbind() const;

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

private:
    void Destroy();

private:
    uint m_handle = 0;
};
