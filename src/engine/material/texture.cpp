#include "engine/material/texture.h"

#include <fmt/format.h>
#include "engine/api/gl.h"

Texture::Texture(const Image& image, bool generateMipLevelsIfNeed, std::string& error, Result& isSuccess) noexcept {
    glGenTextures(1, &m_handle);
    isSuccess.value = Create(image, generateMipLevelsIfNeed, error);
}

Texture::~Texture() noexcept {
    Destroy();
}

void Texture::Bind(uint unit) const noexcept {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::Unbind(uint unit) const noexcept {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::Create(const Image& image, bool generateMipLevelsIfNeed, std::string& error) noexcept {
    const auto textureFormat = image.header.format;
    if ((!GLApi::isDXTSupported) && (textureFormat >= PixelFormat::FIRST_COMPRESSED)) {
        error = fmt::format("DXT compressed texture format ({}) not supported", ToStr(textureFormat));
        return false;
    }

    GLenum internalFormat, format, type;
    if (!image.header.GetOpenGLFormat(internalFormat, format, type)) {
        error = fmt::format("unsupported texture format: {}", ToStr(textureFormat));
        return false;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glBindTexture(GL_TEXTURE_2D, m_handle);

    GLint level=0;
    auto mipImage = image;
    const GLint border = 0; // This value must be 0
    do {
        const auto width = static_cast<GLsizei>(mipImage.header.width);
        const auto height = static_cast<GLsizei>(mipImage.header.height);
        if (textureFormat < PixelFormat::FIRST_COMPRESSED) {
            glTexImage2D(GL_TEXTURE_2D, level, static_cast<GLint>(internalFormat), width, height, border, format, type, mipImage.data);
        } else {
            GLsizei imageSize = static_cast<GLsizei>(mipImage.header.GetSize());
            glCompressedTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, border, imageSize, mipImage.data);
        }
        ++level;
    } while(mipImage.GetNextMiplevel(mipImage));

    bool isOneLevel = (level == 1);
    bool isEmpty = (image.data == nullptr);
    if (generateMipLevelsIfNeed && isOneLevel && !isEmpty) {
        glGenerateMipmap(GL_TEXTURE_2D);
        isOneLevel = false;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, isOneLevel ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::Destroy() noexcept {
    if (m_handle != 0) {
        glDeleteTextures(1, &m_handle);
        m_handle = 0;
    }
}
