#include "middleware/node_editor/node.h"

#include <spdlog/spdlog.h>
#include <imgui_node_editor.h>

#include "engine/gui/widgets.h"


namespace ne = ax::NodeEditor;

void BasePin::Draw(bool connected, uint8_t alpha) const noexcept {
    ne::BeginPin(ne::PinId(this), m_isInput ? ne::PinKind::Input : ne::PinKind::Output);
        gui::NodeIcon(math::Size(24, 24), gui::IconType::Circle, connected, math::Color(48, 220, 48, alpha), math::Color(32, 32, 32, alpha));
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
}

void BaseNode::AddInPin(BasePin* item) {
    item->m_isInput = true;
    m_inPins.push_back(item);
}

void BaseNode::AddOutPin(BasePin* item) {
    item->m_isInput = false;
    m_outPins.push_back(item);
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
            pin->Draw(false, alpha);
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
            if (DrawSettings()) {
                m_needUpdate = true;
            }
        } else {
            DrawPreview();
        }
        ImGui::SameLine();

        ImGui::BeginGroup();
        for (const auto* pin: m_outPins) {
            pin->Draw(false, alpha);
        }
        ImGui::EndGroup();

    ne::EndNode();
}
