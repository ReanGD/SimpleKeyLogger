#pragma once

#include <noise.h>

#include "engine/material/image.h"
#include "middleware/node_editor/node.h"


class Texture;
class BaseNoiseNode : public BaseNode {
protected:
    BaseNoiseNode(noise::module::Module* module, const std::string& name);

protected:
    bool OnSetSourceNode(BaseNode* srcNode, BasePin* dstPin, bool checkOnly) noexcept override;
    void OnDelSourceNode(BaseNode* srcNode, BasePin* dstPin) noexcept override;
    bool Update(std::string& error) noexcept override;
    bool CheckIsConsistency() noexcept override;
    bool OnDrawSettings() noexcept override;
    void OnDrawPreview() noexcept override;

    virtual bool OnDrawSettingsImpl() noexcept { return false; }
    virtual const noise::module::Module** GetSourceModules() noexcept = 0;

    uint32_t m_previewSize = 128;
    uint32_t m_texSize = 128;
    std::shared_ptr<Texture> m_texturePreview = nullptr;
    noise::module::Module* m_module = nullptr;
};

class BillowNode : public BaseNoiseNode, private noise::module::Billow {
public:
    BillowNode();
    bool OnDrawSettingsImpl() noexcept override;
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

class CheckerboardNode : public BaseNoiseNode, private noise::module::Checkerboard {
public:
    CheckerboardNode();
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

class ConstNode : public BaseNoiseNode, private noise::module::Const {
public:
    ConstNode();
    bool OnDrawSettingsImpl() noexcept override;
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

class CylindersNode : public BaseNoiseNode, private noise::module::Cylinders {
public:
    CylindersNode();
    bool OnDrawSettingsImpl() noexcept override;
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

class PerlinNode : public BaseNoiseNode, private noise::module::Perlin {
public:
    PerlinNode();
    bool OnDrawSettingsImpl() noexcept override;
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

class RidgedMultiNode : public BaseNoiseNode, private noise::module::RidgedMulti {
public:
    RidgedMultiNode();
    bool OnDrawSettingsImpl() noexcept override;
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

class SpheresNode : public BaseNoiseNode, private noise::module::Spheres {
public:
    SpheresNode();
    bool OnDrawSettingsImpl() noexcept override;
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

class VoronoiNode : public BaseNoiseNode, private noise::module::Voronoi {
public:
    VoronoiNode();
    bool OnDrawSettingsImpl() noexcept override;
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

class AbsNode : public BaseNoiseNode, private noise::module::Abs {
public:
    AbsNode();
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

class ClampNode : public BaseNoiseNode, private noise::module::Clamp {
public:
    ClampNode();
    bool OnDrawSettingsImpl() noexcept override;
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

// TODO: Curve
class ExponentNode : public BaseNoiseNode, private noise::module::Exponent {
public:
    ExponentNode();
    bool OnDrawSettingsImpl() noexcept override;
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

class ScaleBiasNode : public BaseNoiseNode, private noise::module::ScaleBias {
public:
    ScaleBiasNode();
    bool OnDrawSettingsImpl() noexcept override;
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};

class SelectNode : public BaseNoiseNode, private noise::module::Select {
public:
    SelectNode();
    bool OnDrawSettingsImpl() noexcept override;
    const noise::module::Module** GetSourceModules() noexcept override { return m_pSourceModule; }
};
