#pragma once

#include "engine/material/texture.h"
#include "engine/material/renderbuffer.h"


class Framebuffer : Noncopyable {
public:
    Framebuffer();
    ~Framebuffer();

    bool Create(uint32_t width, uint32_t height, std::string& error) noexcept;

    uint GetColorBufferHandle() const noexcept {
        return m_colorBuffer.GetHandle();
    }

    void Bind() const;
    void Unbind() const;

private:
    void Destroy();

private:
    Texture m_colorBuffer;
    Renderbuffer m_depthBuffer;
    uint m_handle = 0;
};
