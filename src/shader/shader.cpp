#include "shader/shader.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <filesystem>
#include <fmt/format.h>


std::pair<GLchar*, GLint> read(const char* filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        return std::make_pair(nullptr, 0);
    }

    struct stat sb;
    fstat(fd, &sb);
    std::size_t length = static_cast<std::size_t>(sb.st_size);

    GLchar* data = new GLchar[length];
    read(fd, data, length);
    close(fd);

    return std::make_pair(data, length);
}

nonstd::expected<GLuint, std::string> LoadShader(std::filesystem::path&& filepath, GLenum shaderType) {
    auto [data, length] = read(filepath.c_str());

    if (data == nullptr) {
        return nonstd::make_unexpected(fmt::format("Сouldn't read the shader from the file: '{}'", filepath.c_str()));
    }

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &data, &length);
    glCompileShader(shader);

    delete[] data;

    GLint success;    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        return nonstd::make_unexpected(fmt::format("Couldn't compile the shader from the file '{}', error: '{}'", filepath.c_str(), infoLog));
    }

    return shader;
}

Shader::Shader(GLuint shaderHandle)
    : m_shaderHandle(shaderHandle) {

}

void Shader::Bind() {
    glUseProgram(m_shaderHandle);
}

GLint Shader::GetUniformLocation(const GLchar *name) {
    return glGetUniformLocation(m_shaderHandle, name);
}

void Shader::Delete() {
    glDeleteProgram(m_shaderHandle);
}

nonstd::expected<Shader, std::string> Shader::Create(const std::string& vertexShaderName, const std::string& fragmentShaderName) {
    const auto root = std::filesystem::current_path() / "data" / "shaders";

    auto vertexShader = LoadShader(root / (vertexShaderName + ".glsl"), GL_VERTEX_SHADER);
    if (!vertexShader) {
        return nonstd::make_unexpected(vertexShader.error());
    }
    auto fragmentShader = LoadShader(root / (fragmentShaderName + ".glsl"), GL_FRAGMENT_SHADER);
    if (!fragmentShader) {
        glDeleteShader(*vertexShader);
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
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        return nonstd::make_unexpected(fmt::format(
            "Couldn't compile the shader program from vertex shader '{}' and fragment '{}', error: '{}'",
            vertexShaderName, fragmentShaderName, infoLog));
    }

    return Shader(shaderProgram);
}
