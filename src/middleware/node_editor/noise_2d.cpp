#include "middleware/node_editor/noise_2d.h"

#include "engine/common/exception.h"
#include "middleware/node_editor/noise_3d.h"


BaseNoise2DNode::BaseNoise2DNode(const std::string& name)
    : PreviewNode(name) {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    AddOutPin(new BasePin(PinType::Noise2D, 0));
    SetIsFull(false);
}

void BaseNoise2DNode::Update() {
    if (!GetIsFull()) {
        return;
    }

    UpdatePreview(this);
}

PlaneNode::PlaneNode()
    : BaseNoise2DNode("Plane") {
}

double PlaneNode::GetValue(double u, double v) const {
    return m_model.GetValue(u, v);
}

void PlaneNode::SetSourceNode(BaseNode* srcNode, BasePin* dstPin) {
    auto* srcNoiseNode = dynamic_cast<BaseNoise3DNode*>(srcNode);
    if (!srcNoiseNode) {
        throw EngineError("BaseNoise3DNode incoming node is expected");
    }

    m_model.SetModule(srcNoiseNode->GetModule());

    BaseNode::SetSourceNode(srcNode, dstPin);
}

SphereNode::SphereNode()
    : BaseNoise2DNode("Sphere") {
}

double SphereNode::GetValue(double u, double v) const {
    return m_model.GetValue(v, u);
}

void SphereNode::SetSourceNode(BaseNode* srcNode, BasePin* dstPin) {
    auto* srcNoiseNode = dynamic_cast<BaseNoise3DNode*>(srcNode);
    if (!srcNoiseNode) {
        throw EngineError("BaseNoise3DNode incoming node is expected");
    }

    m_model.SetModule(srcNoiseNode->GetModule());

    BaseNode::SetSourceNode(srcNode, dstPin);
}

CylinderNode::CylinderNode()
    : BaseNoise2DNode("Cylinder") {
}

double CylinderNode::GetValue(double u, double v) const {
    return m_model.GetValue(u, v);
}

void CylinderNode::SetSourceNode(BaseNode* srcNode, BasePin* dstPin) {
    auto* srcNoiseNode = dynamic_cast<BaseNoise3DNode*>(srcNode);
    if (!srcNoiseNode) {
        throw EngineError("BaseNoise3DNode incoming node is expected");
    }

    m_model.SetModule(srcNoiseNode->GetModule());

    BaseNode::SetSourceNode(srcNode, dstPin);
}