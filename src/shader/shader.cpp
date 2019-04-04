#include "shader/shader.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

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

nonstd::expected<GLuint, std::string> LoadShader(const char* filepath, GLenum shaderType) {
    auto [data, length] = read(filepath);

    if (data == nullptr) {
        return nonstd::make_unexpected(fmt::format("Сouldn't read the shader from the file: '{}'", filepath));
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
        return nonstd::make_unexpected(fmt::format("Couldn't compile the shader from the file '{}', error: '{}'", filepath, infoLog));
    }

    return shader;
}
