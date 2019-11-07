#include "engine/material/shader.h"

#include <ucl++.h>
#include <fstream>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>

#include "engine/common/exception.h"
#include "engine/api/gl.h"


static bool ReadFile(const char* filepath, std::string& data, std::string& error) {
    std::ifstream ifs(filepath, std::ifstream::in);
    if(!ifs) {
        error = fmt::format("couldn't open file '{}', error: {}", filepath, strerror(errno));
        return false;
    }

    ifs.seekg(0, std::ios::end);
    data.reserve(static_cast<size_t>(ifs.tellg()));
    ifs.seekg(0, std::ios::beg);

    data.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    return true;
}

bool ParseMaterial(const std::string& data, std::string& shaderSrc, std::string& error) {
    auto cfg = ucl::Ucl::parse(data, error, UCL_DUPLICATE_ERROR);
    if (!cfg) {
        return false;
    }

    for (const auto &it :cfg) {
        if ((it.key() == "geometry") || (it.key() == "vertex") || (it.key() == "fragment")) {
            shaderSrc = it.string_value();
            if (shaderSrc.empty()) {
                error = fmt::format("empty shader section");
                return false;
            }
            return true;
        }
    }

    error = fmt::format("not found shader section");
    return false;
}

static GLuint LoadShader(std::filesystem::path&& filepath, GLenum shaderType, std::string& error) {
    std::string data;
    if (!ReadFile(filepath.c_str(), data, error)) {
        error = fmt::format("couldn't read material: '{}'", error);
        return 0;
    }

    std::string shaderSrc;
    if (!ParseMaterial(data, shaderSrc, error)) {
        error = fmt::format("couldn't parse material from file '{}': '{}'", filepath.c_str(), error);
        return 0;
    }

    GLuint shader = glCreateShader(shaderType);
    const GLchar* glData = static_cast<const GLchar *>(shaderSrc.c_str());
    auto length = static_cast<GLint>(shaderSrc.length());
    glShaderSource(shader, 1, &glData, &length);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        error = fmt::format("couldn't compile the shader from the file '{}', error: '{}'", filepath.c_str(), infoLog);
        return 0;
    }

    return shader;
}

Shader::Shader(const PrivateArg&, uint handle)
    : m_handle(handle) {

}

Shader::~Shader() {
    Destroy();
}

std::shared_ptr<Shader> Shader::Create(const std::string& vertexShaderName, const std::string& fragmentShaderName) {
    const auto root = std::filesystem::current_path() / "materials";

    std::string error;
    auto vertexShader = LoadShader(root / (vertexShaderName + ".mat"), GL_VERTEX_SHADER, error);
    if (vertexShader == 0) {
        throw EngineError(error);
    }
    auto fragmentShader = LoadShader(root / (fragmentShaderName + ".mat"), GL_FRAGMENT_SHADER, error);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        throw EngineError(error);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderProgram, 1024, NULL, infoLog);
        throw EngineError("couldn't compile the shader program from vertex shader '{}' and fragment '{}', error: '{}'",
            vertexShaderName, fragmentShaderName, infoLog);
    }

    return std::make_shared<Shader>(Shader::PrivateArg{}, shaderProgram);
}

std::shared_ptr<Shader> Shader::Create(
    const std::string& vertexShaderName,
    const std::string& geometryShaderName,
    const std::string& fragmentShaderName) {

    const auto root = std::filesystem::current_path() / "materials";

    std::string error;
    auto vertexShader = LoadShader(root / (vertexShaderName + ".mat"), GL_VERTEX_SHADER, error);
    if (vertexShader == 0) {
        throw EngineError(error);
    }
    auto geometryShader = LoadShader(root / (geometryShaderName + ".mat"), GL_GEOMETRY_SHADER, error);
    if (geometryShader == 0) {
        glDeleteShader(vertexShader);
        throw EngineError(error);
    }
    auto fragmentShader = LoadShader(root / (fragmentShaderName + ".mat"), GL_FRAGMENT_SHADER, error);
    if (fragmentShader == 0) {
        glDeleteShader(geometryShader);
        glDeleteShader(vertexShader);
        throw EngineError(error);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(geometryShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderProgram, 1024, NULL, infoLog);
        throw EngineError("couldn't compile the shader program from vertex shader '{}' and fragment '{}', error: '{}'",
            vertexShaderName, fragmentShaderName, infoLog);
    }

    return std::make_shared<Shader>(Shader::PrivateArg{}, shaderProgram);
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

void Shader::Destroy() {
    if (m_handle != 0) {
        glDeleteProgram(m_handle);
        m_handle = 0;
    }
}
