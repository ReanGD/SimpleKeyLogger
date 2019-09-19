#pragma once

#include <string>
#include "engine/material/image.h"


class Texture {
public:
    Texture() = default;
    ~Texture() = default;

    bool Create(const Image& image, std::string& error) noexcept;
    bool Load(const std::string& path, std::string& error);

    uint GetHandle() const noexcept {
        return m_handle;
    }

    void Bind(uint unit) const noexcept;
    void Unbind(uint unit) const noexcept;
    void AttachToFramebuffer() const noexcept;
    void Destroy();
private:
    uint m_handle = 0;
};
