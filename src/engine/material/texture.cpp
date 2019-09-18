#include "engine/material/texture.h"

#include <filesystem>
#include "engine/api/gl.h"
#include "engine/material/image_loader.h"


void Texture::Create(uint32_t width, uint32_t height, PixelFormat format, void* data) noexcept {
    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    // TODO: fix
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint level = 0;
    GLint outFormat = ToOpenGL(format);
    GLenum inFormat = static_cast<GLenum>(ToOpenGL(format));
    GLenum inType = GL_UNSIGNED_BYTE;
    GLint border = 0; // This value must be 0

    glTexImage2D(GL_TEXTURE_2D, level, outFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), border, inFormat, inType, data);

    glBindTexture(GL_TEXTURE_2D, 0);

    Destroy();
    m_handle = handle;
}

bool Texture::Load(const std::string& path, std::string& error) {
    const auto fullPath = std::filesystem::current_path() / "assets" / "textures" / path;

    bool is16bit;
    int width, height, channels;

    void *data = LoadImage(fullPath.c_str(), width, height, channels, is16bit, error);
    if(data == nullptr) {
        return false;
    }

    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    // TODO: fix
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint outFormat = GL_RGB;
    switch(channels) {
        case 1: outFormat = GL_RED;  break;
        case 2: outFormat = GL_RG;   break;
        case 3: outFormat = GL_RGB;  break;
        case 4: outFormat = GL_RGBA; break;
    }
    GLenum inFormat = static_cast<GLenum>(outFormat);
    GLenum inType = (is16bit) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE;
    GLint border = 0; // This value must be 0
    glTexImage2D(GL_TEXTURE_2D, 0, outFormat, width, height, border, inFormat, inType, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    FreeImage(data);
    glBindTexture(GL_TEXTURE_2D, 0);

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
