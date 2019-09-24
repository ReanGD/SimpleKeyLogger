#include "engine/material/framebuffer.h"

#include <fmt/format.h>
#include "engine/api/gl.h"
#include "engine/material/texture_manager.h"

static std::string FramebufferStatusResultToString(GLenum value) {
    switch (value) {
    case GL_FRAMEBUFFER_COMPLETE: return "GL_FRAMEBUFFER_COMPLETE";
    case GL_FRAMEBUFFER_UNDEFINED: return "GL_FRAMEBUFFER_UNDEFINED";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
    case GL_FRAMEBUFFER_UNSUPPORTED: return "GL_FRAMEBUFFER_UNSUPPORTED";
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
    default: return fmt::format("unknown value '{}'", value);
    }
}


Framebuffer::Framebuffer() {
    glGenFramebuffers(1, &m_handle);
    glGenRenderbuffers(1, &m_renderbufferHandle);
}

Framebuffer::~Framebuffer() {
    Destroy();
}

bool Framebuffer::Create(uint32_t width, uint32_t height, std::string& error) noexcept {
    bool result = true;

    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    m_colorBuffer = TextureManager::Get().Create(ImageHeader(width, height, PixelFormat::R8G8B8), error);
    if(!m_colorBuffer) {
        error = fmt::format("Can't create colorbuffer for framebuffer: {}", error);
        return false;
    }

    Bind();

    GLint mipmapLvl = 0;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer->m_handle, mipmapLvl);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbufferHandle);

    auto err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (err != GL_FRAMEBUFFER_COMPLETE) {
        result = false;
        error = fmt::format("framebuffer is not complete: {}", FramebufferStatusResultToString(err));
    }

    Unbind();

    return result;
}

void Framebuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
}

void Framebuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Destroy() {
    if (m_renderbufferHandle != 0) {
        glDeleteRenderbuffers(1, &m_renderbufferHandle);
        m_renderbufferHandle = 0;
    }

    if (m_handle != 0) {
        glDeleteFramebuffers(1, &m_handle);
        m_handle = 0;
    }
}
