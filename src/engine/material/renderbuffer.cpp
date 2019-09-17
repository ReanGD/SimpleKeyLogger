#include "engine/material/renderbuffer.h"

#include "engine/api/gl.h"


Renderbuffer::Renderbuffer() {
    glGenRenderbuffers(1, &m_handle);
}

Renderbuffer::~Renderbuffer() {
    Destroy();
}

void Renderbuffer::Create(uint32_t width, uint32_t height) const noexcept {
    glBindRenderbuffer(GL_RENDERBUFFER, m_handle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderbuffer::AttachToFramebuffer() const noexcept {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_handle);
}

void Renderbuffer::Destroy() {
    if (m_handle != 0) {
        glDeleteRenderbuffers(1, &m_handle);
        m_handle = 0;
    }
}
