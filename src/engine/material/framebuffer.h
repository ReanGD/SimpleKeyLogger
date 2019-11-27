#pragma once

#include <memory>
#include "engine/common/noncopyable.h"


class Texture;
class Framebuffer : Noncopyable {
public:
    Framebuffer() = default;
    ~Framebuffer();

    void Bind(uint32_t width, uint32_t height);
    std::shared_ptr<Texture> Unbind() const;

private:
    void Create();
    void Destroy();

private:
    uint m_handle = 0;
    uint m_renderbufferHandle = 0;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    std::shared_ptr<Texture> m_colorBuffer = nullptr;
};
