#include "engine/material/shader.h"

#include <glm/gtc/type_ptr.hpp>
#include "engine/api/gl.h"
#include "engine/material/uniform_buffer.h"


Shader::Shader(const PrivateArg&, uint32_t id, uint handle)
    : m_id(id)
    , m_handle(handle) {

}

Shader::~Shader() {
    Destroy();
}

void Shader::Bind() const {
    glUseProgram(m_handle);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

std::shared_ptr<UniformBufferDecl> Shader::GetUBDecl(const char* name) {
    GLuint ubIndex = glGetUniformBlockIndex(m_handle, name);
    if (ubIndex == GL_INVALID_INDEX) {
        return nullptr;
    }

    int ubSize, ubVarCountI;
    glGetActiveUniformBlockiv(m_handle, ubIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &ubSize);
    glGetActiveUniformBlockiv(m_handle, ubIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &ubVarCountI);

    auto ubVarCount = static_cast<size_t>(ubVarCountI);
    uint *indices = new uint[ubVarCount];
    int *offsets = new int[ubVarCount];
    std::shared_ptr<void> _(nullptr, [&indices, &offsets](...){
        delete[] indices;
        delete[] offsets;
    });

    glGetActiveUniformBlockiv(m_handle, ubIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, reinterpret_cast<int*>(indices));
    glGetActiveUniformsiv(m_handle, ubVarCountI, indices, GL_UNIFORM_OFFSET, offsets);

    char nameBuf[256];
    std::unordered_map<std::string, size_t> offsetMap;
    for (size_t i=0; i!=ubVarCount; ++i) {
        int nameLen = 0;
        glGetActiveUniformName(m_handle, indices[i], sizeof(nameBuf), &nameLen, nameBuf);
        offsetMap[std::string(nameBuf, nameBuf + nameLen)] = static_cast<size_t>(offsets[i]);
    }

    return std::make_shared<UniformBufferDecl>(ubIndex, static_cast<size_t>(ubSize), std::move(offsetMap));
}

void Shader::SetBool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(m_handle, name), static_cast<int>(value));
}

void Shader::SetInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(m_handle, name), value);
}

void Shader::SetFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(m_handle, name), value);
}

void Shader::SetVec2(const char* name, const glm::vec2& vec) const {
    glUniform2fv(glGetUniformLocation(m_handle, name), 1, glm::value_ptr(vec));
}

void Shader::SetVec2(const char* name, float x, float y) const {
    glUniform2f(glGetUniformLocation(m_handle, name), x, y);
}

void Shader::SetVec3(const char* name, const glm::vec3& vec) const {
    glUniform3fv(glGetUniformLocation(m_handle, name), 1, glm::value_ptr(vec));
}

void Shader::SetVec3(const char* name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_handle, name), x, y, z);
}

void Shader::SetVec4(const char* name, const glm::vec4& vec) const {
    glUniform4fv(glGetUniformLocation(m_handle, name), 1, glm::value_ptr(vec));
}

void Shader::SetVec4(const char* name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(m_handle, name), x, y, z, w);
}

void Shader::SetMat2(const char* name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(m_handle, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetMat3(const char* name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(m_handle, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetMat4(const char* name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_handle, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetColor3(const char* name, math::Color3f value) const {
    glUniform3fv(glGetUniformLocation(m_handle, name), 1, value.value);
}

void Shader::SetColor4(const char* name, math::Color4f value) const {
    glUniform4fv(glGetUniformLocation(m_handle, name), 1, value.value);
}

void Shader::Destroy() {
    if (m_handle != 0) {
        glDeleteProgram(m_handle);
    }
}
