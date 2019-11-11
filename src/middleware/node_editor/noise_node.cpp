#include "middleware/node_editor/noise_node.h"

#include <imgui_node_editor.h>
#include <imgui_internal.h>

#include "engine/gui/widgets.h"
#include "middleware/node_editor/noiseutils.h"


namespace ne = ax::NodeEditor;

BaseNoiseNode::BaseNoiseNode(noise::module::Module* module, const std::string& name)
    : PreviewNode(name)
    , m_module(module) {
}

bool BaseNoiseNode::OnSetSourceNode(BaseNode* srcNode, BasePin* dstPin, bool checkOnly) {
    auto* srcNoiseNode = dynamic_cast<BaseNoiseNode*>(srcNode);
    if (!srcNoiseNode) {
        return false;
    }
    auto index = static_cast<int>(dstPin->GetUserIndex());
    if ((index >= m_module->GetSourceModuleCount()) || (index < 0)) {
        return false;
    }

    const noise::module::Module** sourceModules = GetSourceModules();
    if (sourceModules == nullptr) {
        return false;
    }

    if (!checkOnly) {
        sourceModules[index] = srcNoiseNode->m_module;
    }

    return true;
}

void BaseNoiseNode::OnDelSourceNode(BaseNode* srcNode, BasePin* dstPin) {
    auto* srcNoiseNode = dynamic_cast<BaseNoiseNode*>(srcNode);
    if (!srcNoiseNode) {
        return;
    }
    auto index = static_cast<int>(dstPin->GetUserIndex());
    if ((index >= m_module->GetSourceModuleCount()) || (index < 0)) {
        return;
    }

    const noise::module::Module** sourceModules = GetSourceModules();
    if (sourceModules == nullptr) {
        return;
    }
    sourceModules[index] = nullptr;
}

void BaseNoiseNode::Update() {
    if (!GetIsFull()) {
        return;
    }

    UpdatePreview(m_module);
}

bool BaseNoiseNode::CheckIsConsistency() noexcept {
    const noise::module::Module** sourceModules = GetSourceModules();
    if (sourceModules == nullptr) {
        return false;
    }

    for (auto i=0; i!=m_module->GetSourceModuleCount(); ++i) {
        if (sourceModules[i] == nullptr) {
            return false;
        }
    }

    return true;
}

bool BaseNoiseNode::DrawSettings() {
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::PushItemWidth(180);

    bool changed = OnDrawSettingsImpl();

    ImGui::PopItemWidth();
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

    return changed;
}

AbsNode::AbsNode()
    : BaseNoiseNode(this, "Abs") {
    AddInPin(new BasePin(PinType::Noise, 0));
    AddOutPin(new BasePin(PinType::Noise, 0));
    SetIsFull(false);
}

ClampNode::ClampNode()
    : BaseNoiseNode(this, "Clamp") {
    AddInPin(new BasePin(PinType::Noise, 0));
    AddOutPin(new BasePin(PinType::Noise, 0));
    SetIsFull(false);
}

bool ClampNode::OnDrawSettingsImpl() {
    bool changed = false;

    changed |= gui::InputScalar("Lower bound", m_lowerBound, gui::Step(0.01, 0.1), gui::Range(-1.0, m_upperBound - 0.01), "%.2f");
    changed |= gui::InputScalar("Upper bound", m_upperBound, gui::Step(0.01, 0.1), gui::Range(m_lowerBound + 0.01, 1.0), "%.2f");

    if (changed) {
        SetBounds(m_lowerBound, m_upperBound);
    }

    return changed;
}

ExponentNode::ExponentNode()
    : BaseNoiseNode(this, "Exponent") {
    AddInPin(new BasePin(PinType::Noise, 0));
    AddOutPin(new BasePin(PinType::Noise, 0));
    SetIsFull(false);
}

bool ExponentNode::OnDrawSettingsImpl() {
    bool changed = false;

    changed |= gui::InputScalar("Exponent", m_exponent, gui::Step(0.01, 0.1), "%.2f");

    return changed;
}

InvertNode::InvertNode()
    : BaseNoiseNode(this, "Invert") {
    AddInPin(new BasePin(PinType::Noise, 0));
    AddOutPin(new BasePin(PinType::Noise, 0));
    SetIsFull(false);
}

ScaleBiasNode::ScaleBiasNode()
    : BaseNoiseNode(this, "ScaleBias") {
    AddInPin(new BasePin(PinType::Noise, 0));
    AddOutPin(new BasePin(PinType::Noise, 0));
    SetIsFull(false);
}

bool ScaleBiasNode::OnDrawSettingsImpl() {
    bool changed = false;

    changed |= gui::InputScalar("Bias", m_bias, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Scale", m_scale, gui::Step(0.1, 1.0), "%.1f");

    return changed;
}

AddNode::AddNode()
    : BaseNoiseNode(this, "Add") {
    AddInPin(new BasePin(PinType::Noise, 0));
    AddInPin(new BasePin(PinType::Noise, 1));
    AddOutPin(new BasePin(PinType::Noise, 0));
    SetIsFull(false);
}

SelectNode::SelectNode()
    : BaseNoiseNode(this, "Select") {
    AddInPin(new BasePin(PinType::Noise, 0));
    AddInPin(new BasePin(PinType::Noise, 1));
    AddInPin(new BasePin(PinType::Noise, 2, math::Color(220, 48, 48)));
    AddOutPin(new BasePin(PinType::Noise, 0));
    SetIsFull(false);
}

bool SelectNode::OnDrawSettingsImpl() {
    bool changed = false;

    changed |= gui::InputScalar("Edge falloff", m_edgeFalloff, gui::Step(0.01, 0.1), "%.2f");
    changed |= gui::InputScalar("Lower bound", m_lowerBound, gui::Step(0.01, 0.1), gui::Range(-1.0, m_upperBound - 0.01), "%.2f");
    changed |= gui::InputScalar("Upper bound", m_upperBound, gui::Step(0.01, 0.1), gui::Range(m_lowerBound + 0.01, 1.0), "%.2f");

    if (changed) {
        SetBounds(m_lowerBound, m_upperBound);
    }

    return changed;
}
