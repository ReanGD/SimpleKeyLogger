#include "engine/material/framebuffer.h"

#include <fmt/format.h>
#include "engine/api/gl.h"

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
}

Framebuffer::~Framebuffer() {
    Destroy();
}

bool Framebuffer::Create(uint32_t width, uint32_t height, std::string& error) noexcept {
    bool result = true;
    if(!m_colorBuffer.Create(Image(width, height, PixelFormat::R8G8B8, nullptr), error)) {
        return false;
    }

    m_depthBuffer.Create(width, height);

    Bind();

    m_colorBuffer.AttachToFramebuffer();
    m_depthBuffer.AttachToFramebuffer();

    auto err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (err != GL_FRAMEBUFFER_COMPLETE) {
        result = false;
        error = fmt::format("Framebuffer is not complete: {}", FramebufferStatusResultToString(err));
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
    if (m_handle != 0) {
        m_colorBuffer.Destroy();
        glDeleteFramebuffers(1, &m_handle);
        m_handle = 0;
    }
}
