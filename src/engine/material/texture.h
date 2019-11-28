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
    Texture(uint32_t id, const ImageView& image, bool generateMipLevelsIfNeed, const PrivateArg&);
    ~Texture() noexcept;

    uint32_t GetId() const noexcept { return m_id; }

    void Update(const ImageView& image, bool generateMipLevels = true);

    void Bind(uint unit) const noexcept;
    void Unbind(uint unit) const noexcept;

private:
    void Create(const ImageView& image, bool generateMipLevelsIfNeed);
    void Destroy() noexcept;

private:
    const uint32_t m_id = 0;
    uint m_handle = 0;
    ImageHeader m_header;
};

