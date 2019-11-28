#include "engine/material/shader_manager.h"

#include <ucl++.h>

#include "engine/api/gl.h"
#include "engine/common/path.h"
#include "engine/material/shader.h"
#include "engine/common/exception.h"
#include "engine/common/hash_combine.h"


std::shared_ptr<Shader> ShaderManager::Create(const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath) {

    auto vertexShader = LoadShader(vertexShaderPath, ShaderType::Vertex);
    auto fragmentShader = LoadShader(fragmentShaderPath, ShaderType::Fragment);
    auto key = ProgramCacheKey{vertexShader, 0,  fragmentShader};
    if (auto it=m_programCache.find(key); it!=m_programCache.cend()) {
        return it->second;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderProgram, 1024, NULL, infoLog);
        throw EngineError("couldn't compile the shader program from vertex '{}' and fragment shader '{}', error: '{}'",
            vertexShaderPath.c_str(), fragmentShaderPath.c_str(), infoLog);
    }

    auto result = std::make_shared<Shader>(Shader::PrivateArg{}, ++m_lastId, shaderProgram);
    m_programCache[key] = result;
    return result;
}

std::shared_ptr<Shader> ShaderManager::Create(const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& geometryShaderPath,
    const std::filesystem::path& fragmentShaderPath) {

    auto vertexShader = LoadShader(vertexShaderPath, ShaderType::Vertex);
    auto geometryShader = LoadShader(geometryShaderPath, ShaderType::Geometry);
    auto fragmentShader = LoadShader(fragmentShaderPath, ShaderType::Fragment);
    auto key = ProgramCacheKey{vertexShader, geometryShader, fragmentShader};
    if (auto it=m_programCache.find(key); it!=m_programCache.cend()) {
        return it->second;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderProgram, 1024, NULL, infoLog);
        throw EngineError("couldn't compile the shader program from vertex '{}', geometry '{}' and fragment shader '{}', error: '{}'",
            vertexShaderPath.c_str(), geometryShaderPath.c_str(), fragmentShaderPath.c_str(), infoLog);
    }

    auto result = std::make_shared<Shader>(Shader::PrivateArg{}, ++m_lastId, shaderProgram);
    m_programCache[key] = result;
    return result;
}

std::string ShaderManager::ParseMaterial(const std::string& data) {
    std::string error;
    auto cfg = ucl::Ucl::parse(data, error, UCL_DUPLICATE_ERROR);
    if (!cfg) {
        throw EngineError(error);
    }

    for (const auto &it :cfg) {
        if ((it.key() == "geometry") || (it.key() == "vertex") || (it.key() == "fragment")) {
            auto result = it.string_value();
            if (result.empty()) {
                throw EngineError("empty shader section");
            }
            return result;
        }
    }

    throw EngineError("not found shader section");
}

uint ShaderManager::CreateShader(const std::string& data, ShaderType shaderType) {
    GLenum glShaderType;

    switch (shaderType) {
    case ShaderType::Geometry:
        glShaderType = GL_GEOMETRY_SHADER;
        break;
    case ShaderType::Vertex:
        glShaderType = GL_VERTEX_SHADER;
        break;
    case ShaderType::Fragment:
        glShaderType = GL_FRAGMENT_SHADER;
        break;
    default:
        throw EngineError("wrong shader type value {}", static_cast<uint8_t>(shaderType));
    }

    GLuint handle = glCreateShader(glShaderType);
    const GLchar* glData = static_cast<const GLchar *>(data.c_str());
    auto length = static_cast<GLint>(data.length());
    glShaderSource(handle, 1, &glData, &length);
    glCompileShader(handle);

    GLint success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(handle, 1024, NULL, infoLog);
        throw EngineError("couldn't compile the shader: '{}'", infoLog);
    }

    return handle;
}

uint ShaderManager::LoadShader(const std::filesystem::path& path, ShaderType shaderType) {
    try {
        auto fullPath = FileManager::Get().GetRealPath(path);

        auto key = ShaderCacheKey{fullPath, shaderType};
        if (auto it = m_shaderCache.find(key); it != m_shaderCache.cend()) {
            return it->second;
        }

        auto matData = FileManager::Get().ReadFullFile(fullPath);
        auto shaderData = ParseMaterial(matData);
        uint handle = CreateShader(shaderData, shaderType);
        m_shaderCache[key] = handle;
        return handle;
    } catch(const std::exception& e) {
        throw EngineError("failed to create shader from file '{}', error: {}", path.c_str(), e.what());
    }
}


std::size_t ShaderManager::ShaderCacheKey::operator()(const ShaderManager::ShaderCacheKey& value) const {
    std::size_t h = 0;
    hash_combine(h, value.path.string());
    hash_combine(h, static_cast<uint8_t>(value.shaderType));
    return h;
}

bool ShaderManager::ShaderCacheKey::operator==(const ShaderManager::ShaderCacheKey& other) const {
    return ((shaderType == other.shaderType) && (path == other.path));
}


std::size_t ShaderManager::ProgramCacheKey::operator()(const ShaderManager::ProgramCacheKey& value) const {
    std::size_t h = 0;
    hash_combine(h, value.geometryHandle);
    hash_combine(h, value.vertexHandle);
    hash_combine(h, value.fragmentHandle);
    return h;
}

bool ShaderManager::ProgramCacheKey::operator==(const ShaderManager::ProgramCacheKey& other) const {
    return ((geometryHandle == other.geometryHandle) && (vertexHandle == other.vertexHandle) && (fragmentHandle == other.fragmentHandle));
}
