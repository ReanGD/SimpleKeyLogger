#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <memory>
#include <nonstd/expected.hpp>

class Shader
{
private:
    Shader() = delete;
    Shader(GLuint shaderHandle);
public:
    ~Shader() = default;

    void Bind();
    GLint GetUniformLocation(const GLchar *name);
    void Delete();

    static nonstd::expected<Shader, std::string> Create(const std::string& vertexShaderName, const std::string& fragmentShaderName);
private:
    GLuint m_shaderHandle;
};
