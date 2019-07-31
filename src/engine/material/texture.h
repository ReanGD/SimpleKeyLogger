#pragma once

#include <string>


class Texture {
public:
    Texture() = default;
    ~Texture() = default;

    void Create(uint32_t width, uint32_t height) noexcept;
    bool Load(const std::string& path, std::string& error);

    void Bind(uint unit) const noexcept;
    void Unbind(uint unit) const noexcept;
    void AttachToFramebuffer() const noexcept;
    void Destroy();
private:
    uint m_handle = 0;
};
