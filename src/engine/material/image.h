#pragma once

#include <string>
#include <functional>
#include "engine/common/noncopyable.h"


enum class PixelFormat : uint8_t {
    R8 = 1,       //  8 bpp
    R16,          // 16 bpp
    R32,          // 32 bpp (float)

    R8G8,         // 16 bpp
    R16G16,       // 32 bpp
    R32G32,       // 64 = 32*2 bpp (float)

    R5G6B5,       // 16 bpp
    R8G8B8,       // 24 bpp
    R16G16B16,    // 48 = 16*3 bpp
    R32G32B32,    // 96 = 32*3 bpp (float)

    R4G4B4A4,     // 16 bpp
    R5G5B5A1,     // 16 bpp
    R8G8B8A8,     // 32 bpp
    R16G16B16A16, // 64 = 16*4 bpp
    R32G32B32A32, // 128 = 32*4 bpp (float)

    DXT1_RGB,     // 4 bpp (compressed)
    DXT1_RGBA,    // 4 bpp (compressed, 1 bit alpha)
    DXT3_RGBA,    // 8 bpp (compressed)
    DXT5_RGBA,    // 8 bpp (compressed)

    RG8 = R8G8,
    RG16 = R16G16,
    RG32 = R32G32,

    RGB8 = R8G8B8,
    RGB16 = R16G16B16,
    RGB32 = R32G32B32,

    RGBA8 = R8G8B8A8,
    RGBA16 = R16G16B16A16,
    RGBA32 = R32G32B32A32,

    FIRST_COMPRESSED = DXT1_RGB,
};

std::string ToStr(PixelFormat value);

struct ImageHeader {
    ImageHeader() = default;
    ImageHeader(uint32_t width, uint32_t height, PixelFormat format);

    bool operator==(const ImageHeader& other) const noexcept;
    bool operator!=(const ImageHeader& other) const noexcept;

    bool GetOpenGLFormat(uint& internalFormat, uint& format, uint& type) const noexcept;
    size_t GetSize() const noexcept;

    uint32_t width = 0;
    uint32_t height = 0;
    PixelFormat format = PixelFormat::R8G8B8;
};

struct ImageView {
    ImageView() = default;
    ImageView(const ImageHeader& header, uint32_t mipCount, void* data);
    ~ImageView() = default;

    bool GetNextMiplevel(ImageView& image) const noexcept;

    ImageHeader header;
    uint32_t mipCount = 0;
    void* data = nullptr;
};

struct Image : Noncopyable {
    using Deleter = std::function<void (void*)>;
    static void Free(void* data);

    Image() = default;
    Image(const ImageHeader& header, uint32_t mipCount, void* data, Deleter deleter);
    ~Image();

    // With memory allocation and without mip livels
    void Create(const ImageHeader& header);
    bool Load(const char *filename, bool verticallyFlip, std::string& error);
    void Destroy();

    ImageView view;
    Deleter deleter = nullptr;
};
