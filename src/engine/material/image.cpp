#include "engine/material/image.h"

#include "engine/api/gl.h"


int ToOpenGL(PixelFormat value) {
    switch (value) {
        case PixelFormat::UNCOMPRESSED_R8G8B8: return GL_RGB;
        case PixelFormat::UNCOMPRESSED_R8G8B8A8: return GL_RGBA;
        default: return GL_RGB;
    }
}
