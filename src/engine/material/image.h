#pragma once

enum class PixelFormat {
    UNCOMPRESSED_R8G8B8,            // 24 bpp
    UNCOMPRESSED_R8G8B8A8,          // 32 bpp
};

int ToOpenGL(PixelFormat value);
