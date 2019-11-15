#pragma once

#include <noise.h>

#include "middleware/node_editor/preview_node.h"


class BaseNoise3DNode : public PreviewNode {
protected:
    BaseNoise3DNode(noise::module::Module* module, const std::string& name);

public:
    void SetSourceNode(BaseNode* srcNode, BasePin* dstPin) override;
    const noise::module::Module& GetModule() const { return *m_module; }

protected:
    void Update() override;
    bool DrawSettings() override;

    virtual bool OnDrawSettings() { return false; }

private:
    noise::module::Module* m_module = nullptr;
};

class BillowNode : public BaseNoise3DNode, private noise::module::Billow {
public:
    BillowNode();
    bool OnDrawSettings() override;
};

class CheckerboardNode : public BaseNoise3DNode, private noise::module::Checkerboard {
public:
    CheckerboardNode();
};

class ConstNode : public BaseNoise3DNode, private noise::module::Const {
public:
    ConstNode();
    bool OnDrawSettings() override;
};

class CylindersNode : public BaseNoise3DNode, private noise::module::Cylinders {
public:
    CylindersNode();
    bool OnDrawSettings() override;
};

class PerlinNode : public BaseNoise3DNode, private noise::module::Perlin {
public:
    PerlinNode();
    bool OnDrawSettings() override;
};

class RidgedMultiNode : public BaseNoise3DNode, private noise::module::RidgedMulti {
public:
    RidgedMultiNode();
    bool OnDrawSettings() override;
};

class SpheresNode : public BaseNoise3DNode, private noise::module::Spheres {
public:
    SpheresNode();
    bool OnDrawSettings() override;
};

class VoronoiNode : public BaseNoise3DNode, private noise::module::Voronoi {
public:
    VoronoiNode();
    bool OnDrawSettings() override;
};

class AbsNode : public BaseNoise3DNode, private noise::module::Abs {
public:
    AbsNode();
};

class ClampNode : public BaseNoise3DNode, private noise::module::Clamp {
public:
    ClampNode();
    bool OnDrawSettings() override;
};

// TODO: Curve
class ExponentNode : public BaseNoise3DNode, private noise::module::Exponent {
public:
    ExponentNode();
    bool OnDrawSettings() override;
};

class InvertNode : public BaseNoise3DNode, private noise::module::Invert {
public:
    InvertNode();
};

class ScaleBiasNode : public BaseNoise3DNode, private noise::module::ScaleBias {
public:
    ScaleBiasNode();
    bool OnDrawSettings() override;
};

// TODO: Terrace

class AddNode : public BaseNoise3DNode, private noise::module::Add {
public:
    AddNode();
};

class MaxNode : public BaseNoise3DNode, private noise::module::Max {
public:
    MaxNode();
};

class MinNode : public BaseNoise3DNode, private noise::module::Min {
public:
    MinNode();
};

class MultiplyNode : public BaseNoise3DNode, private noise::module::Multiply {
public:
    MultiplyNode();
};

class PowerNode : public BaseNoise3DNode, private noise::module::Power {
public:
    PowerNode();
};

class SelectNode : public BaseNoise3DNode, private noise::module::Select {
public:
    SelectNode();
    bool OnDrawSettings() override;
};
