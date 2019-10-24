#pragma once

#include <noise.h>

#include "engine/material/image.h"
#include "middleware/node_editor/node.h"


class Texture;
class BaseNoiseNode : public BaseNode {
protected:
    BaseNoiseNode(const noise::module::Module* module, const std::string& name);

public:
    const noise::module::Module& GetModule() const noexcept { return *m_module; }

protected:
    bool Update(std::string& error) noexcept override;
    void DrawPreview() noexcept override;

    bool m_isFull = true;
    Image m_imagePreview;
    std::shared_ptr<Texture> m_texturePreview = nullptr;
    const noise::module::Module* m_module = nullptr;
};

class BillowNode : public BaseNoiseNode, private noise::module::Billow {
public:
    BillowNode();
    bool DrawSettings() noexcept override;
};

class CheckerboardNode : public BaseNoiseNode, private noise::module::Checkerboard {
public:
    CheckerboardNode();
    bool DrawSettings() noexcept override;
};

class PerlinNode : public BaseNoiseNode, private noise::module::Perlin {
public:
    PerlinNode();
    bool DrawSettings() noexcept override;
};

class ScaleBiasNode : public BaseNoiseNode, private noise::module::ScaleBias {
public:
    ScaleBiasNode();
    bool OnIncomingLink(BasePin* src, BasePin* dst, bool checkOnly) noexcept override;
    bool DrawSettings() noexcept override;
};
