#include "mesh/mesh.h"

#include <stdexcept>

#define GLEW_STATIC
#include <GL/glew.h>


const VertexDecl VertexPNTC::vDecl = {
    {0, glm::vec3::length()}, // layout (location = 0) in vec3 vPosition;
    {1, glm::vec3::length()}, // layout (location = 1) in vec3 vNormal;
    {2, glm::vec3::length()}, // layout (location = 2) in vec3 vTangent;
    {3, glm::vec2::length()}, // layout (location = 3) in vec2 vTexCoord;
};

// see: glGetAttribLocation
VertexDecl::VertexDecl(const std::initializer_list<Layout>& layouts) {
    if (layouts.size() >= 16) {
        throw std::runtime_error("Number of available layers exceeded");
    }

    for (const auto& layout: layouts) {
        m_layouts[m_layoutsCnt] = layout;
        m_layoutsCnt++;
        m_vertexSize+=layout.elementCnt;
    }
}

void VertexDecl::Bind() const {
    const GLenum type = GL_FLOAT;
    const GLboolean normalized = GL_FALSE;

 	char* pointer = nullptr;
    auto stride = static_cast<GLsizei>(m_vertexSize * sizeof(GLfloat));
    for(uint i=0; i!=m_layoutsCnt; i++) {
        auto index = static_cast<GLuint>(m_layouts[i].index);
        uint elementCnt = m_layouts[i].elementCnt;

        glVertexAttribPointer(index, static_cast<GLint>(elementCnt), type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
        pointer += (elementCnt * sizeof(GLfloat));
    }
}

DataBuffer::DataBuffer(uint target, const void* data, size_t size) {
    glGenBuffers(1, &m_handle);
    glBindBuffer(static_cast<GLenum>(target), m_handle);
    glBufferData(static_cast<GLenum>(target), static_cast<GLsizeiptr>(size), static_cast<const GLvoid *>(data), GL_STATIC_DRAW);
    glBindBuffer(static_cast<GLenum>(target), 0);
}

void DataBuffer::Delete() {
    if (m_handle != 0) {
        glDeleteBuffers(1, &m_handle);
        m_handle = 0;
    }
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

Mesh::Mesh(const VertexDecl& vDecl, const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer)
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

void Mesh::Bind() const {
    glBindVertexArray(m_handle);
}

void Mesh::Unbind() const {
    glBindVertexArray(0);
}

void Mesh::Draw() const {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexBuffer.Count()), static_cast<GLenum>(m_indexBuffer.Type()), 0);
}

void Mesh::Delete() {
    if (m_handle != 0) {
        glDeleteVertexArrays(1, &m_handle);
        m_handle = 0;
    }

    m_vertexBuffer.Delete();
    m_indexBuffer.Delete();
}
