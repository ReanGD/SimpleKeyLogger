#pragma once

#include <string>


enum class PixelFormat : uint8_t {
    R8 = 1,       //  8 bpp
    R16,          // 16 bpp
    R8G8,         // 16 bpp
    R5G6B5,       // 16 bpp
    R5G5B5A1,     // 16 bpp
    R4G4B4A4,     // 16 bpp
    R8G8B8,       // 24 bpp
    R16G16,       // 32 bpp
    R8G8B8A8,     // 32 bpp
    R32,          // 32 bpp (float)
    R16G16B16,    // 48 = 16*3 bpp
    R16G16B16A16, // 64 = 16*4 bpp
    R32G32,       // 64 = 32*2 bpp (float)
    R32G32B32,    // 96 = 32*3 bpp (float)
    R32G32B32A32, // 128 = 32*4 bpp (float)
    DXT1_RGB,     // 4 bpp (compressed)
    DXT1_RGBA,    // 4 bpp (compressed, 1 bit alpha)
    DXT3_RGBA,    // 8 bpp (compressed)
    DXT5_RGBA,    // 8 bpp (compressed)

    RG8 = R8G8,
    RGB8 = R8G8B8,
    RGBA8 = R8G8B8A8,
    RG16 = R16G16,
    RGB16 = R16G16B16,
    RGBA16 = R16G16B16A16,
    RG32 = R32G32,
    RGB32 = R32G32B32,
    RGBA32 = R32G32B32A32,
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
