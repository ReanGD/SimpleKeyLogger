#include "engine/scene/geometry_node.h"

#include "engine/api/gl.h"
#include "engine/common/exception.h"


const VertexDecl VertexP::vDecl = {
    {0, glm::vec3::length()}, // layout (location = 0) in vec3 vPosition;
};

const VertexDecl VertexPNTC::vDecl = {
    {0, glm::vec3::length()}, // layout (location = 0) in vec3 vPosition;
    {1, glm::vec3::length()}, // layout (location = 1) in vec3 vNormal;
    {2, glm::vec3::length()}, // layout (location = 2) in vec3 vTangent;
    {3, glm::vec2::length()}, // layout (location = 3) in vec2 vTexCoord;
};

// see: glGetAttribLocation
VertexDecl::VertexDecl(const std::initializer_list<Layout>& layouts) {
    if (layouts.size() >= 16) {
        throw EngineError("Number of available layers exceeded");
    }

    for (const auto& layout: layouts) {
        m_layouts[m_layoutsCount] = layout;
        m_layoutsCount++;
        m_vertexCount+=layout.elementCnt;
    }
}

void VertexDecl::Bind() const {
    const GLenum type = GL_FLOAT;
    const GLboolean normalized = GL_FALSE;

 	char* pointer = nullptr;
    auto stride = static_cast<GLsizei>(Size());
    for(uint i=0; i!=m_layoutsCount; i++) {
        auto index = static_cast<GLuint>(m_layouts[i].index);
        uint elementCnt = m_layouts[i].elementCnt;

        glVertexAttribPointer(index, static_cast<GLint>(elementCnt), type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
        pointer += (elementCnt * sizeof(GLfloat));
    }
}

DataBuffer::DataBuffer(uint target, size_t size)
    : m_size(size) {
    glGenBuffers(1, &m_handle);
    glBindBuffer(static_cast<GLenum>(target), m_handle);
    glBufferData(static_cast<GLenum>(target), static_cast<GLsizeiptr>(size), nullptr, GL_STREAM_DRAW);
    glBindBuffer(static_cast<GLenum>(target), 0);
}

DataBuffer::DataBuffer(uint target, const void* data, size_t size)
    : m_size(size) {
    glGenBuffers(1, &m_handle);
    glBindBuffer(static_cast<GLenum>(target), m_handle);
    glBufferData(static_cast<GLenum>(target), static_cast<GLsizeiptr>(size), static_cast<const GLvoid *>(data), GL_STATIC_DRAW);
    glBindBuffer(static_cast<GLenum>(target), 0);
}

void* DataBuffer::Lock(uint target) const noexcept {
    glBindBuffer(static_cast<GLenum>(target), m_handle);
    return glMapBuffer(static_cast<GLenum>(target), GL_WRITE_ONLY);
}

bool DataBuffer::Unlock(uint target) const noexcept {
    bool result = glUnmapBuffer(static_cast<GLenum>(target)) == GL_TRUE;
    glBindBuffer(static_cast<GLenum>(target), 0);

    return result;
}

void DataBuffer::Destroy() {
    if (m_handle != 0) {
        glDeleteBuffers(1, &m_handle);
        m_handle = 0;
    }
}

VertexBuffer::VertexBuffer(size_t size)
    : DataBuffer(GL_ARRAY_BUFFER, size) {

}

VertexBuffer::VertexBuffer(const void* data, size_t size)
    : DataBuffer(GL_ARRAY_BUFFER, data, size) {

}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void* VertexBuffer::Lock() const noexcept {
    return DataBuffer::Lock(GL_ARRAY_BUFFER);
}

bool VertexBuffer::Unlock() const noexcept {
    return DataBuffer::Unlock(GL_ARRAY_BUFFER);
}

IndexBuffer::IndexBuffer(const uint16_t* data, size_t size)
    : DataBuffer(GL_ELEMENT_ARRAY_BUFFER, data, size)
    , m_type(GL_UNSIGNED_SHORT)
    , m_count(static_cast<uint>(size/sizeof(*data))) {

}

IndexBuffer::IndexBuffer(const uint32_t* data, size_t size)
    : DataBuffer(GL_ELEMENT_ARRAY_BUFFER, data, size)
    , m_type(GL_UNSIGNED_INT)
    , m_count(static_cast<uint>(size/sizeof(*data))) {

}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void* IndexBuffer::Lock() const noexcept {
    return DataBuffer::Lock(GL_ELEMENT_ARRAY_BUFFER);
}

bool IndexBuffer::Unlock() const noexcept {
    return DataBuffer::Unlock(GL_ELEMENT_ARRAY_BUFFER);
}

uint32_t Counter::m_lastId = 0;

GeometryNode::GeometryNode(const VertexDecl& vDecl, const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer)
    : m_vDecl(vDecl)
    , m_vertexBuffer(vertexBuffer)
    , m_indexBuffer(indexBuffer) {

    glGenVertexArrays(1, &m_handle);

    Bind();
    m_vertexBuffer.Bind();
    m_vDecl.Bind();
    m_indexBuffer.Bind();
    Unbind();

    m_vertexBuffer.Unbind();
    m_indexBuffer.Unbind();
}

GeometryNode::~GeometryNode() {
    Destroy();
}

void GeometryNode::Bind() const {
    glBindVertexArray(m_handle);
}

void GeometryNode::Unbind() const {
    glBindVertexArray(0);
}

uint32_t GeometryNode::Draw() const {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexBuffer.Count()), static_cast<GLenum>(m_indexBuffer.Type()), 0);
    return m_indexBuffer.Count() / 3;
}

void GeometryNode::Destroy() {
    if (m_handle != 0) {
        glDeleteVertexArrays(1, &m_handle);
        m_handle = 0;
    }

    m_vertexBuffer.Destroy();
    m_indexBuffer.Destroy();
}

Lines::Lines(const VertexDecl& vDecl, const VertexBuffer& vertexBuffer)
    : m_vertexCount(static_cast<uint32_t>(vertexBuffer.Size()/vDecl.Size()))
    , m_vDecl(vDecl)
    , m_vertexBuffer(vertexBuffer) {

    glGenVertexArrays(1, &m_handle);

    Bind();
    m_vertexBuffer.Bind();
    m_vDecl.Bind();
    Unbind();

    m_vertexBuffer.Unbind();
}

Lines::~Lines() {
    Destroy();
}

void Lines::Bind() const {
    glBindVertexArray(m_handle);
}

void Lines::Unbind() const {
    glBindVertexArray(0);
}

uint32_t Lines::Draw() const {
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_vertexCount));
    return m_vertexCount - 1;
}

void Lines::Destroy() {
    if (m_handle != 0) {
        glDeleteVertexArrays(1, &m_handle);
        m_handle = 0;
    }

    m_vertexBuffer.Destroy();
}
