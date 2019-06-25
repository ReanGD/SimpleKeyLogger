#include "engine/material/uniform_buffer.h"

#include <cstring>
#include <algorithm>

#define GLEW_STATIC
#include <GL/glew.h>


UniformBuffer::UniformBuffer(size_t size)
    : m_size(size)
    , m_buffer(reinterpret_cast<uint8_t*>(malloc(m_size))) {
    std::memset(m_buffer, 0, m_size);
    glGenBuffers(1, &m_handle);
}

UniformBuffer::~UniformBuffer() {
    if (m_handle != 0) {
        glDeleteBuffers(1, &m_handle);
        m_handle = 0;
    }

    if (m_buffer != nullptr) {
        free(m_buffer);
        m_buffer = nullptr;
    }

    m_size = 0;
}

void UniformBuffer::Sync() {
    glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(m_size), m_buffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::Bind(uint index) const {
    glBindBufferBase(GL_UNIFORM_BUFFER, index, m_handle);
}

const size_t UniformBufferDecl::InvalidOffset = std::numeric_limits<size_t>::max();

UniformBufferDecl::UniformBufferDecl(uint index, size_t size, std::unordered_map<std::string, size_t>&& offsets)
    : m_index(index)
    , m_size(size)
    , m_offsets(std::move(offsets)) {

}

size_t UniformBufferDecl::GetOffset(const std::string& name) const noexcept {
    const auto it = m_offsets.find(name);
    if (it != m_offsets.end()) {
        return it->second;
    }

    return UniformBufferDecl::InvalidOffset;
}
