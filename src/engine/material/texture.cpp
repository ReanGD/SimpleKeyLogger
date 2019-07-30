#include "engine/material/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>
#include <fmt/format.h>
#include "engine/common/glapi.h"

bool Texture::Create(uint32_t width, uint32_t height, std::string& /*error*/) {
    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    // TODO: fix
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint level = 0;
    GLint outFormat = GL_RGB;
    GLint inFormat = GL_RGB;
    GLenum inType = GL_UNSIGNED_BYTE;
    void *data = nullptr;
    GLint border = 0; // This value must be 0

    glTexImage2D(GL_TEXTURE_2D, level, outFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), border, inFormat, inType, data);

    glBindTexture(GL_TEXTURE_2D, 0);

    Destroy();
    m_handle = handle;

    return true;
}

bool Texture::Load(const std::string& path, std::string& error) {
    const auto fullPath = std::filesystem::current_path() / "assets" / "textures" / path;
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    FILE *f = stbi__fopen(fullPath.c_str(), "rb");
    if (f == nullptr) {
        error = fmt::format("Failed to open a texture file '{}'", fullPath.c_str());
        return false;
    }

    stbi__context s;
    stbi__start_file(&s, f);
    long pos = ftell(f);
    bool is16bit = (stbi__is_16_main(&s) == 1);
    fseek(f,pos,SEEK_SET);

    void *data = nullptr;
    if (is16bit) {
        data = stbi__load_and_postprocess_16bit(&s, &width, &height, &channels, STBI_default);
    } else {
        data = stbi__load_and_postprocess_8bit(&s, &width, &height, &channels, STBI_default);
    }
    fclose(f);

    if (data == nullptr) {
        error = fmt::format("Failed to create a texture from file '{}', error: '{}'", fullPath.c_str(), stbi_failure_reason());
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
    GLenum inFormat = outFormat;
    GLenum inType = (is16bit) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE;
    GLint border = 0; // This value must be 0
    glTexImage2D(GL_TEXTURE_2D, 0, outFormat, width, height, border, inFormat, inType, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    Destroy();
    m_handle = handle;

    return true;
}

void Texture::Bind(uint unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::Unbind(uint unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Destroy() {
    if (m_handle != 0) {
        glDeleteTextures(1, &m_handle);
        m_handle = 0;
    }
}
