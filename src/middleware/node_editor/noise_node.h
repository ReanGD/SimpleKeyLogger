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
    bool DrawSettings() noexcept override;
    void DrawPreview() noexcept override;

    virtual bool DrawSettingsImpl() noexcept { return false; }

    bool m_isFull = true;
    Image m_imagePreview;
    std::shared_ptr<Texture> m_texturePreview = nullptr;
    const noise::module::Module* m_module = nullptr;
};

class BillowNode : public BaseNoiseNode, private noise::module::Billow {
public:
    BillowNode();
    bool DrawSettingsImpl() noexcept override;
};

class CheckerboardNode : public BaseNoiseNode, private noise::module::Checkerboard {
public:
    CheckerboardNode();
};

class PerlinNode : public BaseNoiseNode, private noise::module::Perlin {
public:
    PerlinNode();
    bool DrawSettingsImpl() noexcept override;
};

class RidgedMultiNode : public BaseNoiseNode, private noise::module::RidgedMulti {
public:
    RidgedMultiNode();
    bool DrawSettingsImpl() noexcept override;
};

class ScaleBiasNode : public BaseNoiseNode, private noise::module::ScaleBias {
public:
    ScaleBiasNode();
    bool OnIncomingLink(BasePin* src, BasePin* dst, bool checkOnly) noexcept override;
    bool DrawSettingsImpl() noexcept override;
};
