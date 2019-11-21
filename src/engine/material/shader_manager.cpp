#include "engine/material/shader_manager.h"

#include <map>
#include <fstream>
#include <ucl++.h>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>

#include "engine/api/gl.h"
#include "engine/common/exception.h"


enum class ShaderType : uint8_t {
    Vertex = 0,
    Geometry,
    Fragment,
    Unknown
};

enum class UniformType : uint8_t {
    Vec2 = 0,
    Vec3,
    Vec4,
    Unknown
};

static UniformType FromString(const std::string& value) {
    if (value == "vec2") {
        return UniformType::Vec2;
    } else if (value == "vec3") {
        return UniformType::Vec3;
    } else if (value == "vec4") {
        return UniformType::Vec4;
    } else {
        return UniformType::Unknown;
    }
}

struct UniformInfo {
    UniformType type = UniformType::Unknown;
    std::string name;
};

class MaterialParser {
public:
    MaterialParser() = default;
    ~MaterialParser() = default;

    void Parse(const std::filesystem::path& path) {
        m_shaderType = ShaderType::Unknown;
        m_funcName.clear();
        m_funcData.clear();
        m_uniforms.clear();

        if (!std::filesystem::exists(path)) {
            throw EngineError("the material of the path '{}' was not found", path.c_str());
        }

        std::string data;
        try {
            ReadFile(path.c_str(), data);
        } catch(const std::exception& e) {
            throw EngineError("the material of the path '{}' was not read: {}", path.c_str(), e.what());
        }

        std::string error;
        auto root = ucl::Ucl::parse(data, error, UCL_DUPLICATE_ERROR);
        if (!root) {
            throw EngineError("the material of the path '{}' was not parse: {}", path.c_str(), error);
        }

        try {
            ParseRoot(root);
        } catch(const std::exception& e) {
            throw EngineError("the material of the path '{}' was not parse: {}", path.c_str(), e.what());
        }
    }
private:
    void ReadFile(const char* filepath, std::string& data) {
        std::ifstream ifs(filepath, std::ifstream::in);
        if(!ifs) {
            throw EngineError("couldn't open file '{}', error: {}", filepath, strerror(errno));
        }

        ifs.seekg(0, std::ios::end);
        data.reserve(static_cast<size_t>(ifs.tellg()));
        ifs.seekg(0, std::ios::beg);

        data.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    }

    void ParseRoot(const ucl::Ucl& section) {
        for (const auto &it :section) {
            if (it.key() == "material") {
                ParseMaterial(it);
            } else if (it.key() == "vertex") {
                m_shaderType = ShaderType::Vertex;
                m_funcData = it.string_value();
            } else if (it.key() == "geometry") {
                m_shaderType = ShaderType::Geometry;
                m_funcData = it.string_value();
            } else if (it.key() == "fragment") {
                m_shaderType = ShaderType::Fragment;
                m_funcData = it.string_value();
            } else {
                throw EngineError("unknown section: {} with data: {}", it.key(), it.dump());
            }
        }

        if (m_funcName.empty()) {
            throw EngineError("not found function name");
        }
        if (m_funcData.empty()) {
            throw EngineError("not found function data");
        }
    }

    void ParseMaterial(const ucl::Ucl& section) {
        for (const auto& it: section) {
            if (it.key() == "func") {
                m_funcName = it.string_value();
            } else if (it.key() == "uniforms") {
                ParseUniforms(it);
            } else {
                throw EngineError("unknown section: {} with data: {}", it.key(), it.dump());
            }
        }
    }

    void ParseUniforms(const ucl::Ucl& section) {
        for (const auto& uniformIt: section) {
            UniformInfo uniform;
            std::string desc = uniformIt.key();
            if (desc.empty()) {
                throw EngineError("empty uniform description");
            }
            for (const auto& it: uniformIt) {
                if (it.key() == "type") {
                    uniform.type = FromString(it.string_value());
                    if (uniform.type == UniformType::Unknown) {
                        throw EngineError("unknown uniform type: {} in uniform:{}", it.string_value(), desc);
                    }
                } else if (it.key() == "name") {
                    uniform.name = it.string_value();
                } else {
                    throw EngineError("unknown section in uniform:{}: key={}, value={}", desc, it.key(), it.dump());
                }
            }

            if (uniform.name.empty()) {
                throw EngineError("not found name for uniform:{}", desc);
            }
            if (uniform.type == UniformType::Unknown) {
                throw EngineError("not found type for uniform:{}", desc);
            }
            m_uniforms[desc] = uniform;
        }
    }

private:
    ShaderType m_shaderType = ShaderType::Unknown;
    std::string m_funcName;
    std::string m_funcData;
    std::map<std::string, UniformInfo> m_uniforms;
};

void ShaderManager::Create() {
    const auto base = std::filesystem::current_path() / "materials";

    MaterialParser parser1;
    parser1.Parse(base / "frag_base_clr_mat.mat");

    MaterialParser parser2;
    parser1.Parse(base / "frag_light_none.mat");
}
