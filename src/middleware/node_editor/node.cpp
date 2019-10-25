#include "middleware/node_editor/node.h"

#include <spdlog/spdlog.h>
#include <imgui_node_editor.h>

#include "engine/gui/widgets.h"


namespace ne = ax::NodeEditor;

BasePin::BasePin(uint32_t userIndex)
    : m_userIndex(userIndex) {

}

void BasePin::Draw(uint8_t alpha) const noexcept {
    ne::BeginPin(ne::PinId(this), m_isInput ? ne::PinKind::Input : ne::PinKind::Output);
        gui::NodeIcon(math::Size(24, 24), gui::IconType::Circle, IsConnected(), math::Color(48, 220, 48, alpha), math::Color(32, 32, 32, alpha));
    ne::EndPin();
}

BaseNode::BaseNode(const std::string& name)
    : m_name(name) {

}

BaseNode::~BaseNode() {
    for (auto* pin: m_inPins) {
        delete pin;
    }
    m_inPins.clear();

    for (auto* pin: m_outPins) {
        delete pin;
    }
    m_outPins.clear();

    m_LinkedDstNodes.clear();
}

void BaseNode::AddInPin(BasePin* pin) {
    pin->m_isInput = true;
    pin->m_node = this;
    m_inPins.push_back(pin);
}

void BaseNode::AddOutPin(BasePin* pin) {
    pin->m_isInput = false;
    pin->m_node = this;
    m_outPins.push_back(pin);
}

bool BaseNode::AddIncomingLink(BasePin* src, BasePin* dst, bool checkOnly) noexcept {
    if (dst->IsConnected()) {
        return false;
    }

    bool result = OnIncomingLink(src, dst, checkOnly);
    if (!checkOnly) {
        src->AddLink();
        dst->AddLink();
        SetNeedUpdate();
    }

    return result;
}

void BaseNode::LinkDstNode(BaseNode* dst) noexcept {
    m_LinkedDstNodes.insert(dst);
}

void BaseNode::SetNeedUpdate() noexcept {
    m_needUpdate = true;
    for (auto& node: m_LinkedDstNodes) {
        node->SetNeedUpdate();
    }
}

void BaseNode::Draw() noexcept {
    if (m_wrongNode) {
        return;
    }

    auto alpha = static_cast<uint8_t>(ImGui::GetStyle().Alpha * 255.0f);
    ne::NodeId id(this);
    ne::BeginNode(id);
        if (ne::GetDoubleClickedNode() == id) {
            m_drawSettings = !m_drawSettings;
        }

        ImGui::TextUnformatted(m_name.c_str());

        ImGui::BeginGroup();
        for (const auto* pin: m_inPins) {
            pin->Draw(alpha);
        }
        ImGui::EndGroup();

        if (m_needUpdate) {
            std::string error;
            if (OnUpdate(error)) {
                m_needUpdate = false;
            } else {
                m_wrongNode = true;
                spdlog::error("Can't update node: {}", error);
            }
        }

        if (m_drawSettings) {
            if (OnDrawSettings()) {
                SetNeedUpdate();
            }
        } else {
            OnDrawPreview();
        }
        ImGui::SameLine();

        ImGui::BeginGroup();
        for (const auto* pin: m_outPins) {
            pin->Draw(alpha);
        }
        ImGui::EndGroup();

    ne::EndNode();
}
