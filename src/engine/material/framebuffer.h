#pragma once

#include <memory>
#include "engine/material/texture.h"


class Framebuffer : Noncopyable {
public:
    Framebuffer();
    ~Framebuffer();

    bool Create(uint32_t width, uint32_t height, std::string& error) noexcept;

    void Bind() const;
    void Unbind() const;

private:
    void Destroy();

private:
    uint m_handle = 0;
    uint m_renderbufferHandle = 0;
    std::shared_ptr<Texture> m_colorBuffer = nullptr;
};
