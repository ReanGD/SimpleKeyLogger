#include "middleware/node_editor/noise_3d.h"

#include <imgui_node_editor.h>

#include "engine/gui/widgets.h"
#include "engine/common/exception.h"
#include "middleware/node_editor/noiseutils.h"


namespace ne = ax::NodeEditor;

BaseNoise3DNode::BaseNoise3DNode(noise::module::Module* module, const std::string& name)
    : BaseNode(name)
    , m_module(module) {

    AddOutPin(new BasePin(PinType::Noise3D, 0));
}

BaseNoise3DNode::~BaseNoise3DNode() {
    if (m_nodePreview != nullptr) {
        delete m_nodePreview;
        m_nodePreview = nullptr;
    }
}

void BaseNoise3DNode::SetSourceNode(BaseNode* srcNode, BasePin* dstPin) {
    auto* srcNoiseNode = dynamic_cast<BaseNoise3DNode*>(srcNode);
    if (!srcNoiseNode) {
        throw EngineError("BaseNoise3DNode incoming node is expected");
    }

    m_module->SetSourceModule(static_cast<int>(dstPin->GetUserIndex()), srcNoiseNode->GetModule());
    BaseNode::SetSourceNode(srcNode, dstPin);
}

void BaseNoise3DNode::Update() {
    if (m_nodePreview == nullptr) {
        m_nodePreview = new PlaneNode();
        auto dstPin = BasePin(PinType::Noise3D, 0);
        m_nodePreview->SetSourceNode(this, &dstPin);
    }
    m_nodePreview->Update();
}

bool BaseNoise3DNode::DrawSettings() {
    ImGui::PushItemWidth(128);
    bool changed = OnDrawSettings();
    ImGui::PopItemWidth();

    return changed;
}

void BaseNoise3DNode::DrawPreview() {
    if (m_nodePreview) {
        m_nodePreview->DrawPreview();
    }
}

AbsNode::AbsNode()
    : BaseNoise3DNode(this, "Abs") {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    SetIsFull(false);
}

ClampNode::ClampNode()
    : BaseNoise3DNode(this, "Clamp") {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    SetIsFull(false);
}

bool ClampNode::OnDrawSettings() {
    bool changed = false;

    changed |= gui::InputScalar("Lower bound", m_lowerBound, gui::Step(0.01, 0.1), gui::Range(-1.0, m_upperBound - 0.01), "%.2f");
    changed |= gui::InputScalar("Upper bound", m_upperBound, gui::Step(0.01, 0.1), gui::Range(m_lowerBound + 0.01, 1.0), "%.2f");

    if (changed) {
        SetBounds(m_lowerBound, m_upperBound);
    }

    return changed;
}

ExponentNode::ExponentNode()
    : BaseNoise3DNode(this, "Exponent") {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    SetIsFull(false);
}

bool ExponentNode::OnDrawSettings() {
    bool changed = false;

    changed |= gui::InputScalar("Exponent", m_exponent, gui::Step(0.01, 0.1), "%.2f");

    return changed;
}

InvertNode::InvertNode()
    : BaseNoise3DNode(this, "Invert") {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    SetIsFull(false);
}

ScaleBiasNode::ScaleBiasNode()
    : BaseNoise3DNode(this, "ScaleBias") {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    SetIsFull(false);
}

bool ScaleBiasNode::OnDrawSettings() {
    bool changed = false;

    changed |= gui::InputScalar("Bias", m_bias, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Scale", m_scale, gui::Step(0.1, 1.0), "%.1f");

    return changed;
}

AddNode::AddNode()
    : BaseNoise3DNode(this, "Add") {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    AddInPin(new BasePin(PinType::Noise3D, 1));
    SetIsFull(false);
}

MaxNode::MaxNode()
    : BaseNoise3DNode(this, "Max") {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    AddInPin(new BasePin(PinType::Noise3D, 1));
    SetIsFull(false);
}

MinNode::MinNode()
    : BaseNoise3DNode(this, "Min") {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    AddInPin(new BasePin(PinType::Noise3D, 1));
    SetIsFull(false);
}

MultiplyNode::MultiplyNode()
    : BaseNoise3DNode(this, "Multiply") {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    AddInPin(new BasePin(PinType::Noise3D, 1));
    SetIsFull(false);
}

PowerNode::PowerNode()
    : BaseNoise3DNode(this, "Power") {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    AddInPin(new BasePin(PinType::Noise3D, 1));
    SetIsFull(false);
}

SelectNode::SelectNode()
    : BaseNoise3DNode(this, "Select") {
    AddInPin(new BasePin(PinType::Noise3D, 0));
    AddInPin(new BasePin(PinType::Noise3D, 1));
    AddInPin(new BasePin(PinType::Noise3D, 2, math::Color(220, 48, 48)));
    SetIsFull(false);
}

bool SelectNode::OnDrawSettings() {
    bool changed = false;

    changed |= gui::InputScalar("Edge falloff", m_edgeFalloff, gui::Step(0.01, 0.1), "%.2f");
    changed |= gui::InputScalar("Lower bound", m_lowerBound, gui::Step(0.01, 0.1), gui::Range(-1.0, m_upperBound - 0.01), "%.2f");
    changed |= gui::InputScalar("Upper bound", m_upperBound, gui::Step(0.01, 0.1), gui::Range(m_lowerBound + 0.01, 1.0), "%.2f");

    if (changed) {
        SetBounds(m_lowerBound, m_upperBound);
    }

    return changed;
}
