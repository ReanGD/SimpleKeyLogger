#pragma once

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


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
protected:
    DataBuffer() = delete;
    DataBuffer(uint32_t target, const void* data, size_t size);

public:
    ~DataBuffer() = default;

public:
    void Delete();

protected:
    uint32_t m_handle;
};

class VertexBuffer : public DataBuffer {
public:
    VertexBuffer(const void* data, size_t size);

public:
    void Bind() const;
    void Unbind() const;
};

class IndexBuffer : public DataBuffer {
public:
    explicit IndexBuffer(const uint16_t* data, size_t size);
    explicit IndexBuffer(const uint32_t* data, size_t size);

public:
    uint32_t Count() const noexcept {
        return m_count;
    }

    uint32_t Type() const noexcept {
        return m_type;
    }

    void Bind() const;
    void Unbind() const;

private:
    uint32_t m_count;
    uint32_t m_type;
};

class Mesh {
public:
    Mesh() = delete;
    Mesh(const VertexDecl& vDecl, const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer);
    ~Mesh() = default;

public:
    void Bind() const;
    void Unbind() const;
    void Draw() const;
    void Delete();

private:
    uint32_t m_handle;
    VertexDecl m_vDecl;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
};
