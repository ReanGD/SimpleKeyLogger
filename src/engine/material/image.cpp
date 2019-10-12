#include "engine/material/image.h"

#include <fmt/format.h>
#include "engine/api/gl.h"


std::string ToStr(PixelFormat value) {
    switch (value) {
        case PixelFormat::R8: return "R8";
        case PixelFormat::R16: return "R16";
        case PixelFormat::R32: return "R32";

        case PixelFormat::R8G8: return "R8G8";
        case PixelFormat::R16G16: return "R16G16";
        case PixelFormat::R32G32: return "R32G32";

        case PixelFormat::R5G6B5: return "R5G6B5";
        case PixelFormat::R8G8B8: return "R8G8B8";
        case PixelFormat::R16G16B16: return "R16G16B16";
        case PixelFormat::R32G32B32: return "R32G32B32";

        case PixelFormat::R4G4B4A4: return "R4G4B4A4";
        case PixelFormat::R5G5B5A1: return "R5G5B5A1";
        case PixelFormat::R8G8B8A8: return "R8G8B8A8";
        case PixelFormat::R16G16B16A16: return "R16G16B16A16";
        case PixelFormat::R32G32B32A32: return "R32G32B32A32";

        case PixelFormat::DXT1_RGB: return "DXT1_RGB";
        case PixelFormat::DXT1_RGBA: return "DXT1_RGBA";
        case PixelFormat::DXT3_RGBA: return "DXT3_RGBA";
        case PixelFormat::DXT5_RGBA: return "DXT5_RGBA";

        default: return fmt::format("unknown value '{}'", static_cast<uint8_t>(value));
    }
}

ImageHeader::ImageHeader(uint32_t width, uint32_t height, PixelFormat format)
    : width(width)
    , height(height)
    , format(format) {

}

bool ImageHeader::operator==(const ImageHeader& other) const noexcept {
    return ((height == other.height) && (width == other.width) && (format == other.format));
}

bool ImageHeader::operator!=(const ImageHeader& other) const noexcept {
    return !operator==(other);
}

bool ImageHeader::GetOpenGLFormat(uint& internalFormat, uint& inFormat, uint& type) const noexcept {
    static constexpr const uint UNKNOWN = std::numeric_limits<uint>::max();
    internalFormat = UNKNOWN;
    inFormat = UNKNOWN;
    type = UNKNOWN;
    switch (format) {
        case PixelFormat::R8: internalFormat = GL_R8; inFormat = GL_RED; type = GL_UNSIGNED_BYTE; break;
        case PixelFormat::R16: internalFormat = GL_R16; inFormat = GL_RED; type = GL_UNSIGNED_SHORT; break;
        case PixelFormat::R32: internalFormat = GL_R32F; inFormat = GL_RED; type = GL_FLOAT; break;

        case PixelFormat::R8G8: internalFormat = GL_RG8; inFormat = GL_RG; type = GL_UNSIGNED_BYTE; break;          // NONE
        case PixelFormat::R16G16: internalFormat = GL_RG16; inFormat = GL_RG; type = GL_UNSIGNED_SHORT; break;      // NONE
        case PixelFormat::R32G32: internalFormat = GL_RG32F; inFormat = GL_RG; type = GL_FLOAT; break;              // NONE

        case PixelFormat::R5G6B5: internalFormat = GL_RGB565; inFormat = GL_RGB; type = GL_UNSIGNED_SHORT_5_6_5; break;
        case PixelFormat::R8G8B8: internalFormat = GL_RGB8; inFormat = GL_RGB; type = GL_UNSIGNED_BYTE; break;      // NONE
        case PixelFormat::R16G16B16: internalFormat = GL_RGB16; inFormat = GL_RGB; type = GL_UNSIGNED_SHORT; break; // NONE
        case PixelFormat::R32G32B32: internalFormat = GL_RGB32F; inFormat = GL_RGB; type = GL_FLOAT; break;         // NONE

        case PixelFormat::R4G4B4A4: internalFormat = GL_RGBA4; inFormat = GL_RGBA; type = GL_UNSIGNED_SHORT_4_4_4_4; break;   // GL_UNSIGNED_INT_8_8_8_8_REV
        case PixelFormat::R5G5B5A1: internalFormat = GL_RGB5_A1; inFormat = GL_RGBA; type = GL_UNSIGNED_SHORT_5_5_5_1; break; // GL_UNSIGNED_SHORT_1_5_5_5_REV
        case PixelFormat::R8G8B8A8: internalFormat = GL_RGBA8; inFormat = GL_RGBA; type = GL_UNSIGNED_BYTE; break;            // GL_UNSIGNED_INT_8_8_8_8_REV
        case PixelFormat::R16G16B16A16: internalFormat = GL_RGBA16; inFormat = GL_RGBA; type = GL_UNSIGNED_SHORT; break;
        case PixelFormat::R32G32B32A32: internalFormat = GL_RGBA32F; inFormat = GL_RGBA; type = GL_FLOAT; break;

        case PixelFormat::DXT1_RGB: if (GLApi::IsDXTSupported) internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
        case PixelFormat::DXT1_RGBA: if (GLApi::IsDXTSupported) internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
        case PixelFormat::DXT3_RGBA: if (GLApi::IsDXTSupported) internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
        case PixelFormat::DXT5_RGBA: if (GLApi::IsDXTSupported) internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;

        default: return false;
    }

    return true;
}

size_t ImageHeader::GetSize() const noexcept {
    size_t bpp = 0; // Bits per pixel
    switch (format) {
        case PixelFormat::DXT1_RGB:
        case PixelFormat::DXT1_RGBA: bpp = 4; break;
        case PixelFormat::R8:
        case PixelFormat::DXT3_RGBA:
        case PixelFormat::DXT5_RGBA: bpp = 8; break;
        case PixelFormat::R16:
        case PixelFormat::R8G8:
        case PixelFormat::R5G6B5:
        case PixelFormat::R5G5B5A1:
        case PixelFormat::R4G4B4A4: bpp = 16; break;
        case PixelFormat::R8G8B8: bpp = 24; break;
        case PixelFormat::R16G16:
        case PixelFormat::R8G8B8A8:
        case PixelFormat::R32: bpp = 32; break;
        case PixelFormat::R16G16B16: bpp = 48; break;
        case PixelFormat::R16G16B16A16:
        case PixelFormat::R32G32: bpp = 64; break;
        case PixelFormat::R32G32B32: bpp = 96; break;
        case PixelFormat::R32G32B32A32: bpp = 128; break;
        default: break;
    }

    size_t size = width*height*bpp/8;

    // Most compressed formats works on 4x4 blocks,
    // if texture is smaller, minimum size is 8 or 16
    if ((width < 4) && (height < 4)) {
        if ((format == PixelFormat::DXT1_RGB) || (format == PixelFormat::DXT1_RGBA)) {
            size = 8;
        } else if ((format == PixelFormat::DXT3_RGBA) || (format == PixelFormat::DXT5_RGBA)) {
            size = 16;
        }
    }

    return size;
}

Image::Image(const ImageHeader& header, uint32_t mipCount, void* data)
    : header(header)
    , mipCount(mipCount)
    , data(data) {

    if (data == nullptr) {
        mipCount = 0;
    } else if (mipCount == 0) {
        mipCount = 1;
    }
}

bool Image::GetNextMiplevel(Image& image) const noexcept {
    if (mipCount <= 1) {
        return false;
    }

    auto offset = header.GetSize();

    image.header.width = std::max(header.width / 2, 1u);
    image.header.height = std::max(header.height / 2, 1u);
    image.header.format = header.format;
    image.mipCount = mipCount - 1;
    image.data = static_cast<uint8_t*>(data) + offset;

    return true;
}
