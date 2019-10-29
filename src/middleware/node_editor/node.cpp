#include "middleware/node_editor/node.h"

#include <spdlog/spdlog.h>
#include <imgui_node_editor.h>

#include "engine/gui/widgets.h"


namespace ne = ax::NodeEditor;

BasePin::BasePin(PinType pinType, uint32_t userIndex, math::Color color)
    : m_pinType(pinType)
    , m_userIndex(userIndex)
    , m_color(color) {

}

void BasePin::DelLink() noexcept {
    if (m_linkCount > 0) {
        m_linkCount--;
    }
}

void BasePin::Draw(uint8_t alpha) const noexcept {
    ne::BeginPin(ne::PinId(this), m_isInput ? ne::PinKind::Input : ne::PinKind::Output);
        math::Color color = m_color;
        color.SetAlpha(alpha);
        gui::NodeIcon(math::Size(24, 24), gui::IconType::Circle, IsConnected(), color, math::Color(32, 32, 32, alpha));
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
    m_LinkedSrcNodes.clear();
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

    bool result = OnAddIncomingLink(src, dst, checkOnly);
    if (!checkOnly) {
        src->AddLink();
        dst->AddLink();

        m_LinkedSrcNodes.insert(src->GetNode());
        CheckIsFull();
    }

    return result;
}

void BaseNode::DelIncomingLink(BasePin* src, BasePin* dst) noexcept {
    OnDelIncomingLink(src, dst);
    src->DelLink();
    dst->DelLink();
    m_LinkedSrcNodes.erase(src->GetNode());
    CheckIsFull();
}

void BaseNode::AddOutgoingLink(BaseNode* dst) noexcept {
    m_LinkedDstNodes.insert(dst);
}

void BaseNode::DelOutgoingLink(BaseNode* dst) noexcept {
    m_LinkedDstNodes.erase(dst);
}

void BaseNode::SetNeedUpdate() noexcept {

    m_needUpdate = true;
    for (auto& node: m_LinkedDstNodes) {
        node->SetNeedUpdate();
    }
}

void BaseNode::CheckIsFull() noexcept {
    bool isFull = CheckIsConsistency();
    if (isFull) {
        for(const auto& node : m_LinkedSrcNodes) {
            if (!node->GetIsFull()) {
                isFull = false;
                break;
            }
        }
    }

    if (isFull) {
        SetNeedUpdate();
    }

    SetIsFull(isFull);
}

void BaseNode::SetIsFull(bool value) noexcept {
    if (m_isFull == value) {
        return;
    }
    m_isFull = value;
    for (auto& node: m_LinkedDstNodes) {
        node->CheckIsFull();
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
            if (Update(error)) {
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
