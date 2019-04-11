#include "mesh/mesh.h"

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
    auto stride = static_cast<GLsizei>(m_vertexSize * sizeof(GLfloat));
    GLboolean normalized = GL_FALSE;
    GLenum type = GL_FLOAT;

 	char* pointer = nullptr;
    for(uint i=0; i!=m_layoutsCnt; i++) {
        GLuint index = m_layouts[i].index;
        uint elementCnt = m_layouts[i].elementCnt;

        glVertexAttribPointer(index, static_cast<GLint>(elementCnt), type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
        pointer += (elementCnt * sizeof(GLfloat));
    }
}

DataBuffer::DataBuffer(uint target, const void* data, ptrdiff_t size)
    : m_target(target) {

    glGenBuffers(1, &m_handle);
    Bind();
    glBufferData(m_target, size, data, GL_STATIC_DRAW);
    Unbind();
}

void DataBuffer::Bind() const {
    glBindBuffer(m_target, m_handle);
}

void DataBuffer::Unbind() const {
    glBindBuffer(m_target, 0);
}

void DataBuffer::Delete() {
    if (m_handle != 0) {
        glDeleteBuffers(1, &m_handle);
        m_handle = 0;
    }
}

Mesh::Mesh(const VertexDecl& vDecl, const DataBuffer& vertexBuffer, const DataBuffer& indexBuffer)
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

void Mesh::Draw(GLsizei count, GLenum type) const {
    glDrawElements(GL_TRIANGLES, count, type, 0);
}

void Mesh::Delete() {
    if (m_handle != 0) {
        glDeleteVertexArrays(1, &m_handle);
        m_handle = 0;
    }

    m_vertexBuffer.Delete();
    m_indexBuffer.Delete();
}
