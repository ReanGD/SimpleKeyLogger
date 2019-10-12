#pragma once

#include "engine/material/image.h"
#include "engine/common/noncopyable.h"


class Texture : Noncopyable {
    struct Result {
        bool value;
    };
    friend class Gui;
    friend class Framebuffer;
    friend class TextureManager;

public:
    Texture() = delete;
    Texture(const Image& image, bool generateMipLevelsIfNeed, std::string& error, Result& isSuccess) noexcept;
    ~Texture() noexcept;

    // auto generate mip levels
    bool Update(const Image& image, std::string& error) noexcept;
    bool Update(const Image& image, bool generateMipLevels, std::string& error) noexcept;

    void Bind(uint unit) const noexcept;
    void Unbind(uint unit) const noexcept;

private:
    bool Create(const Image& image, bool generateMipLevelsIfNeed, std::string& error) noexcept;
    void Destroy() noexcept;

private:
    uint m_handle = 0;
    const ImageHeader m_header;
};
