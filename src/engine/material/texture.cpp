#include "engine/material/texture.h"

#include <filesystem>
#include <fmt/format.h>
#include "engine/api/gl.h"
#include "engine/material/image_loader.h"


bool Texture::Create(const Image& image, std::string& error) noexcept {
    const auto pFormat = image.format;
    if ((!GLApi::isDXTSupported) && (pFormat >= PixelFormat::FIRST_COMPRESSED)) {
        error = fmt::format("DXT compressed texture format not supported");
        return false;
    }

    GLenum internalFormat, format, type;
    if (!image.GetOpenGLFormat(internalFormat, format, type)) {
        error = fmt::format("Unsupported format: {}", ToStr(image.format));
        return false;
    }

    // TODO: fix
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    GLint level = 0;
    GLint border = 0; // This value must be 0
    const auto width = static_cast<GLsizei>(image.width);
    const auto height = static_cast<GLsizei>(image.height);
    if (pFormat < PixelFormat::FIRST_COMPRESSED) {
        glTexImage2D(GL_TEXTURE_2D, level, static_cast<GLint>(internalFormat), width, height, border, format, type, image.data);
    } else {
        GLsizei imageSize = static_cast<GLsizei>(image.GetSize());
        glCompressedTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, border, imageSize, image.data);
    }

    // TODO: fix
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    Destroy();
    m_handle = handle;

    return true;
}

bool Texture::Load(const std::string& path, std::string& error) {
    const auto fullPath = std::filesystem::current_path() / "assets" / "textures" / path;

    Image image;
    if(!LoadImage(fullPath.c_str(), image, error)) {
        return false;
    }

    const auto pFormat = image.format;
    if ((!GLApi::isDXTSupported) && (pFormat >= PixelFormat::FIRST_COMPRESSED)) {
        error = fmt::format("DXT compressed texture format not supported");
        FreeImage(image.data);
        return false;
    }

    GLenum internalFormat, format, type;
    if (!image.GetOpenGLFormat(internalFormat, format, type)) {
        error = fmt::format("Unsupported format: {}", ToStr(image.format));
        FreeImage(image.data);
        return false;
    }

    // TODO: fix
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    GLint level = 0;
    GLint border = 0; // This value must be 0
    const auto width = static_cast<GLsizei>(image.width);
    const auto height = static_cast<GLsizei>(image.height);
    if (pFormat < PixelFormat::FIRST_COMPRESSED) {
        glTexImage2D(GL_TEXTURE_2D, level, static_cast<GLint>(internalFormat), width, height, border, format, type, image.data);
        glGenerateMipmap(GL_TEXTURE_2D); //!
    } else {
        GLsizei imageSize = static_cast<GLsizei>(image.GetSize());
        glCompressedTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, border, imageSize, image.data);
        // ?
    }

    // TODO: fix
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    FreeImage(image.data);

    Destroy();
    m_handle = handle;

    return true;
}

void Texture::Bind(uint unit) const noexcept {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::Unbind(uint unit) const noexcept {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::AttachToFramebuffer() const noexcept {
    GLint mipmapLvl = 0;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_handle, mipmapLvl);
}

void Texture::Destroy() {
    if (m_handle != 0) {
        glDeleteTextures(1, &m_handle);
        m_handle = 0;
    }
}
