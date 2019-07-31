#pragma once

#include <string>

#include "engine/common/noncopyable.h"


class Renderbuffer : Noncopyable {
public:
    Renderbuffer();
    ~Renderbuffer();

    void Create(uint32_t width, uint32_t height) const noexcept;
    void AttachToFramebuffer() const noexcept;

private:
    void Destroy();

private:
    uint m_handle = 0;
};
