#include "middleware/node_editor/base_editor_node.h"

#include <imgui_node_editor.h>
#include <imgui_internal.h>

#include "engine/gui/widgets.h"
#include "engine/common/exception.h"


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

void BasePin::Draw(uint8_t alpha) const {
    ne::BeginPin(ne::PinId(this), m_isInput ? ne::PinKind::Input : ne::PinKind::Output);
        math::Color color = m_color;
        color.alpha = alpha;
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

void BaseNode::SetSourceNode(BaseNode* srcNode, BasePin* dstPin) {
    dstPin->AddLink();
    m_LinkedSrcNodes.insert(srcNode);
    CheckIsFull();
}

void BaseNode::DelSourceNode(BaseNode* srcNode, BasePin* dstPin) {
    dstPin->DelLink();
    m_LinkedSrcNodes.erase(srcNode);
    CheckIsFull();
}

void BaseNode::AddDestNode(BaseNode* dstNode, BasePin* srcPin) {
    srcPin->AddLink();
    m_LinkedDstNodes.insert(dstNode);
}

void BaseNode::DelDestNode(BaseNode* dstNode, BasePin* srcPin) {
    srcPin->DelLink();
    m_LinkedDstNodes.erase(dstNode);
}

void BaseNode::Draw() {
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

        if (m_needUpdate && m_isFull) {
            Update();
        }
        m_needUpdate = false;

        ImGui::SameLine();
        ImGui::BeginGroup();
        if (m_isFull) {
            DrawPreview();
        }
        if (m_drawSettings && DrawSettings()) {
            SetNeedUpdate();
        }
        ImGui::EndGroup();

        ImGui::SameLine();
        ImGui::BeginGroup();
        for (const auto* pin: m_outPins) {
            pin->Draw(alpha);
        }
        ImGui::EndGroup();

    ne::EndNode();
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

void BaseNode::SetNeedUpdate() noexcept {
    if (m_needUpdate) {
        return;
    }
    m_needUpdate = true;
    for (auto& node: m_LinkedDstNodes) {
        node->SetNeedUpdate();
    }
}

void BaseNode::CheckIsFull() noexcept {
    bool isFull = true;
    for(const auto& pin :m_inPins) {
        if (!pin->IsConnected()) {
            isFull = false;
            break;
        }
    }

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
