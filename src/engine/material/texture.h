#pragma once

#include "engine/material/image.h"
#include "engine/common/noncopyable.h"


namespace gui {
    namespace detail {
        struct TextureGetter;
    }
}
class Texture : Noncopyable {
    struct PrivateArg{};
    friend class Framebuffer;
    friend class TextureManager;
    friend struct gui::detail::TextureGetter;

public:
    Texture() = delete;
    Texture(const ImageView& image, bool generateMipLevelsIfNeed, const PrivateArg&);
    ~Texture() noexcept;

    // auto generate mip levels
    void Update(const ImageView& image);
    void Update(const ImageView& image, bool generateMipLevels);

    void Bind(uint unit) const noexcept;
    void Unbind(uint unit) const noexcept;

private:
    void Create(const ImageView& image, bool generateMipLevelsIfNeed);
    void Destroy() noexcept;

private:
    uint m_handle = 0;
    const ImageHeader m_header;
};
