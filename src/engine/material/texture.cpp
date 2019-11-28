#include "engine/material/texture.h"

#include "engine/api/gl.h"
#include "engine/common/exception.h"


Texture::Texture(uint32_t id, const ImageView& image, bool generateMipLevelsIfNeed, const PrivateArg&)
    : m_id(id)
    , m_header(image.header) {

    Create(image, generateMipLevelsIfNeed);
}

Texture::~Texture() noexcept {
    Destroy();
}

void Texture::Update(const ImageView& image, bool generateMipLevels) {
    if (image.header != m_header) {
        Create(image, generateMipLevels);
        return;
    }

    if (image.data == nullptr) {
        throw EngineError("texture update data are not filled in");
    }

    GLenum internalFormat, format, type;
    if (!image.header.GetOpenGLFormat(internalFormat, format, type)) {
        throw EngineError("unsupported texture format: {}", ToStr(image.header.format));
    }

    glBindTexture(GL_TEXTURE_2D, m_handle);

    const GLint level = 0;
    const GLint xoffset = 0;
    const GLint yoffset = 0;
    const auto width = static_cast<GLsizei>(m_header.width);
    const auto height = static_cast<GLsizei>(m_header.height);
    glTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, format, type, image.data);

    bool isOneLevel = true;
    if (generateMipLevels) {
        glGenerateMipmap(GL_TEXTURE_2D);
        isOneLevel = false;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, isOneLevel ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(uint unit) const noexcept {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::Unbind(uint unit) const noexcept {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Create(const ImageView& image, bool generateMipLevelsIfNeed) {
    Destroy();
    glGenTextures(1, &m_handle);

    const auto textureFormat = image.header.format;
    if ((!GLApi::IsDXTSupported) && (textureFormat >= PixelFormat::FIRST_COMPRESSED)) {
        throw EngineError("DXT compressed texture format ({}) not supported", ToStr(textureFormat));
    }

    GLenum internalFormat, format, type;
    if (!image.header.GetOpenGLFormat(internalFormat, format, type)) {
        throw EngineError("unsupported texture format: {}", ToStr(textureFormat));
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
}

void Texture::Destroy() noexcept {
    if (m_handle != 0) {
        glDeleteTextures(1, &m_handle);
        m_handle = 0;
    }
}
