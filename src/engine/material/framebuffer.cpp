#include "engine/material/framebuffer.h"

#include "engine/api/gl.h"
#include "engine/common/exception.h"
#include "engine/material/texture.h"
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


Framebuffer::~Framebuffer() {
    Destroy();
}

void Framebuffer::Bind(uint32_t width, uint32_t height) {
    if ((m_handle == 0) || (width != m_width) || (height != m_height)) {
        m_width = width;
        m_height = height;
        Destroy();
        Create();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
}

std::shared_ptr<Texture> Framebuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return m_colorBuffer;
}

void Framebuffer::Create() {
    glGenRenderbuffers(1, &m_renderbufferHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height));
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    m_colorBuffer = TextureManager::Get().Create(ImageHeader(m_width, m_height, PixelFormat::R8G8B8));

    glGenFramebuffers(1, &m_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

    GLint mipmapLvl = 0;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer->m_handle, mipmapLvl);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbufferHandle);

    auto err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (err != GL_FRAMEBUFFER_COMPLETE) {
        throw EngineError("framebuffer is not complete: {}", FramebufferStatusResultToString(err));
    }
}

void Framebuffer::Destroy() {
    m_colorBuffer.reset();

    if (m_renderbufferHandle != 0) {
        glDeleteRenderbuffers(1, &m_renderbufferHandle);
        m_renderbufferHandle = 0;
    }

    if (m_handle != 0) {
        glDeleteFramebuffers(1, &m_handle);
        m_handle = 0;
    }
}
