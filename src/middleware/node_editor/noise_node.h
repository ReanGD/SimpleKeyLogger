#pragma once

#include <noise.h>

#include "engine/material/image.h"
#include "middleware/node_editor/node.h"


class Texture;
class BaseNoiseNode : public BaseNode {
protected:
    BaseNoiseNode(const std::string& name);
    ~BaseNoiseNode() = default;

    bool UpdateImpl(const noise::module::Module* module, std::string& error) noexcept;
    void DrawPreview() noexcept override;

    bool m_isFull = true;
    Image m_imagePreview;
    std::shared_ptr<Texture> m_texturePreview = nullptr;
};

class PerlinNode : public BaseNoiseNode, private noise::module::Perlin {
public:
    enum class Quality : uint8_t {
        Fast = 0,
        Std,
        Best,
        Count,
    };
public:
    PerlinNode();
    ~PerlinNode() = default;

    bool Update(std::string& error) noexcept override;
    bool DrawSettings() noexcept override;
};

class BillowNode : public BaseNoiseNode, private noise::module::Billow {
public:
    BillowNode();
    ~BillowNode() = default;

    bool Update(std::string& error) noexcept override;
    bool DrawSettings() noexcept override;
};

class ScaleBiasNode : public BaseNoiseNode, private noise::module::ScaleBias {
public:
    ScaleBiasNode();
    ~ScaleBiasNode() = default;

    void OnIncomingLink(BasePin* src, BasePin* dst) noexcept override;
    bool Update(std::string& error) noexcept override;
    bool DrawSettings() noexcept override;
};
