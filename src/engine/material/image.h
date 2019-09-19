#pragma once

#include <string>


enum class PixelFormat : uint8_t {
    R8 = 1,       //  8 bpp
    R8G8,         // 16 bpp
    R5G6B5,       // 16 bpp
    R5G5B5A1,     // 16 bpp
    R4G4B4A4,     // 16 bpp
    R8G8B8,       // 24 bpp
    R8G8B8A8,     // 32 bpp
    R32,          // 32 bpp (float)
    R32G32B32,    // 32*3 bpp (float)
    R32G32B32A32, // 32*4 bpp (float)
    DXT1_RGB,     // 4 bpp (compressed)
    DXT1_RGBA,    // 4 bpp (compressed, 1 bit alpha)
    DXT3_RGBA,    // 8 bpp (compressed)
    DXT5_RGBA,    // 8 bpp (compressed)

    FIRST_COMPRESSED = DXT1_RGB,
};

std::string ToStr(PixelFormat value);

struct Image {
    Image() = default;
    Image(uint32_t width, uint32_t height, PixelFormat format, void* data);

    bool GetOpenGLFormat(uint& internalFormat, uint& format, uint& type) const noexcept;
    size_t GetSize() const noexcept;

    uint32_t width = 0;
    uint32_t height = 0;
    PixelFormat format = PixelFormat::R8G8B8;
    void* data = nullptr;
};
