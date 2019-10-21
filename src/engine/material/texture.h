#pragma once

#include "engine/material/image.h"
#include "engine/common/noncopyable.h"


namespace gui {
    namespace detail {
        struct TextureGetter;
    }
}
class Texture : Noncopyable {
    struct Result {
        bool value;
    };
    friend class Framebuffer;
    friend class TextureManager;
    friend struct gui::detail::TextureGetter;

public:
    Texture() = delete;
    Texture(const ImageView& image, bool generateMipLevelsIfNeed, std::string& error, Result& isSuccess) noexcept;
    ~Texture() noexcept;

    // auto generate mip levels
    bool Update(const ImageView& image, std::string& error) noexcept;
    bool Update(const ImageView& image, bool generateMipLevels, std::string& error) noexcept;

    void Bind(uint unit) const noexcept;
    void Unbind(uint unit) const noexcept;

private:
    bool Create(const ImageView& image, bool generateMipLevelsIfNeed, std::string& error) noexcept;
    void Destroy() noexcept;

private:
    uint m_handle = 0;
    const ImageHeader m_header;
};
