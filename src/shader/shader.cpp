#include "shader/shader.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <filesystem>
#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>


std::pair<GLchar*, GLint> read(const char* filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        return std::make_pair(nullptr, 0);
    }

    struct stat sb;
    fstat(fd, &sb);
    auto length = static_cast<std::size_t>(sb.st_size);

    GLchar* data = new GLchar[length];
    read(fd, data, length);
    close(fd);

    return std::make_pair(data, length);
}

nonstd::expected<GLuint, std::string> LoadShader(std::filesystem::path&& filepath, GLenum shaderType) {
    auto [data, length] = read(filepath.c_str());

    if (data == nullptr) {
        auto msg = fmt::format("Сouldn't read the shader from the file: '{}'", filepath.c_str());
        return nonstd::make_unexpected(msg);
    }

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &data, &length);
    glCompileShader(shader);

    delete[] data;

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        auto msg = fmt::format("Couldn't compile the shader from the file '{}', error: '{}'", filepath.c_str(), infoLog);
        return nonstd::make_unexpected(msg);
    }

    return shader;
}

Shader::Shader(uint32_t shaderHandle)
    : m_handle(shaderHandle) {

}

void Shader::Bind() const {
    glUseProgram(m_handle);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::Delete() {
    if (m_handle != 0) {
        glDeleteProgram(m_handle);
        m_handle = 0;
    }
}

void Shader::SetBool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(m_handle, name), static_cast<int>(value));
}

void Shader::SetInt(const char* name, int32_t value) const {
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

nonstd::expected<Shader, std::string> Shader::Create(const std::string& vertexShaderName, const std::string& fragmentShaderName) {
    const auto root = std::filesystem::current_path() / "data" / "shaders";

    auto vertexShader = LoadShader(root / (vertexShaderName + ".glsl"), GL_VERTEX_SHADER);
    if (!vertexShader) {
        return nonstd::make_unexpected(vertexShader.error());
    }
    auto fragmentShader = LoadShader(root / (fragmentShaderName + ".glsl"), GL_FRAGMENT_SHADER);
    if (!fragmentShader) {
        glDeleteShader(*fragmentShader);
        return nonstd::make_unexpected(fragmentShader.error());
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, *vertexShader);
    glAttachShader(shaderProgram, *fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(*vertexShader);
    glDeleteShader(*fragmentShader);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderProgram, 1024, NULL, infoLog);
        auto msg = fmt::format("Couldn't compile the shader program from vertex shader '{}' and fragment '{}', error: '{}'",
            vertexShaderName, fragmentShaderName, infoLog);
        return nonstd::make_unexpected(msg);
    }

    return Shader(shaderProgram);
}
