#pragma once

#include <array>
#include <stddef.h>
#include <sys/types.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define GLEW_STATIC
#include <GL/glew.h>


class VertexDecl {
public:
    struct Layout{
        uint8_t index;
        uint8_t elementCnt;
    };

    VertexDecl() = delete;
    VertexDecl(const std::initializer_list<Layout>& layouts);
    ~VertexDecl() = default;

public:
    void Bind() const;

private:
    uint8_t m_layoutsCnt = 0;
    uint8_t m_vertexSize = 0;
    Layout m_layouts[16];
};


struct VertexPNTC {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec2 TexCoord;

    static const VertexDecl vDecl;
};

class DataBuffer {
public:
    DataBuffer() = delete;
    DataBuffer(uint target, const void* data, ptrdiff_t size);
    ~DataBuffer() = default;

public:
    void Bind() const;
    void Unbind() const;
    void Delete();

private:
    uint m_target;
    uint m_handle;
};

class Mesh {
public:
    Mesh() = delete;
    Mesh(const VertexDecl& vDecl, const DataBuffer& vertexBuffer, const DataBuffer& indexBuffer);
    ~Mesh() = default;

public:
    void Bind() const;
    void Unbind() const;
    void Draw(GLsizei count, GLenum type) const;
    void Delete();

private:
    uint m_handle;
    VertexDecl m_vDecl;
    DataBuffer m_vertexBuffer;
    DataBuffer m_indexBuffer;
};
