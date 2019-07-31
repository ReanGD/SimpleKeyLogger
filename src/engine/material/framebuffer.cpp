#include "engine/material/framebuffer.h"

#include "engine/common/glapi.h"


Framebuffer::Framebuffer() {
    glGenFramebuffers(1, &m_handle);
}

Framebuffer::~Framebuffer() {
    Destroy();
}

bool Framebuffer::Create(uint32_t width, uint32_t height, std::string& error) noexcept {
    bool result = true;
    m_colorBuffer.Create(width, height);
    m_depthBuffer.Create(width, height);

    Bind();

    m_colorBuffer.AttachToFramebuffer();
    m_depthBuffer.AttachToFramebuffer();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        result = false;
        error = "Framebuffer is not complete";
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
        glDeleteFramebuffers(1, &m_handle);
        m_handle = 0;
    }
}
