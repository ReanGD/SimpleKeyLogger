#pragma once

#include <filesystem>
#include <unordered_map>
#include "engine/common/noncopyable.h"

enum class ShaderType : uint8_t {
    Geometry = 0,
    Vertex,
    Fragment
};

class Shader;
class ShaderManager : Noncopyable {
public:
    ShaderManager() = default;
    ~ShaderManager() = default;

public:
    static ShaderManager& Get() noexcept {
        static ShaderManager instance;
        return instance;
    }

    std::shared_ptr<Shader> Create(const std::filesystem::path& vertexShaderPath,
        const std::filesystem::path& fragmentShaderPath);

    std::shared_ptr<Shader> Create(const std::filesystem::path& vertexShaderPath,
        const std::filesystem::path& geometryShaderPath,
        const std::filesystem::path& fragmentShaderPath);

private:
    std::string ParseMaterial(const std::string& data);
    uint CreateShader(const std::string& data, ShaderType shaderType);
    uint LoadShader(const std::filesystem::path& path, ShaderType shaderType);

private:
    struct ShaderCacheKey {
        std::filesystem::path path;
        ShaderType shaderType;

        // hash function
        std::size_t operator()(const ShaderCacheKey& value) const;
        bool operator==(const ShaderCacheKey& other) const;
    };

    struct ProgramCacheKey {
        uint vertexHandle;
        uint geometryHandle;
        uint fragmentHandle;

        // hash function
        std::size_t operator()(const ProgramCacheKey& value) const;
        bool operator==(const ProgramCacheKey& other) const;
    };

    std::unordered_map<ShaderCacheKey, uint, ShaderCacheKey> m_shaderCache;
    std::unordered_map<ProgramCacheKey, std::shared_ptr<Shader>, ProgramCacheKey> m_programCache;
    uint32_t m_lastId = 0;
};
