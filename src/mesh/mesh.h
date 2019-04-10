#pragma once

#include <array>
#include <stddef.h>
#include <sys/types.h>

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

namespace VDecls {
    static const VertexDecl PosColorTex = {
        {0, 3}, // layout (location = 0) in vec3 position;
        {1, 3}, // layout (location = 1) in vec3 color;
        {2, 2}, // layout (location = 2) in vec2 texCoord;
    };
}

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
    void Delete();

private:
    uint m_handle;
    VertexDecl m_vDecl;
    DataBuffer m_vertexBuffer;
    DataBuffer m_indexBuffer;
};