#pragma once

#include <nonstd/expected.hpp>

class Texture {
private:
    Texture() = delete;
    Texture(uint32_t handle);
public:
    ~Texture() = default;

    void Bind() const;
    void Unbind() const;
    void Delete();

    static nonstd::expected<Texture, std::string> Create(const std::string& path);
private:
    uint32_t m_handle;
};
