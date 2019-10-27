#include "editor/ui_node_editor.h"

#include <unordered_map>
#include <imgui_node_editor.h>
#include "middleware/node_editor/noise_node.h"


namespace ne = ax::NodeEditor;

struct LinkInfo {
    ne::PinId srcPin;
    ne::PinId dstPin;
};

namespace std {
  template <> struct hash<ne::LinkId> {
    size_t operator()(const ne::LinkId& k) const {
        return size_t(k);
    }
  };
}

static std::unordered_map<ne::LinkId, LinkInfo>  g_links;
static int                  g_NextLinkId = 100;


bool UINodeEditor::Create(std::string& /*error*/) {
    Destroy();
    m_context = ne::CreateEditor();

    m_nodes.push_back(std::make_shared<BillowNode>());
    m_nodes.push_back(std::make_shared<CheckerboardNode>());
    m_nodes.push_back(std::make_shared<PerlinNode>());
    m_nodes.push_back(std::make_shared<RidgedMultiNode>());
    m_nodes.push_back(std::make_shared<ScaleBiasNode>());
    m_nodes.push_back(std::make_shared<SelectNode>());

    return true;
}

void UINodeEditor::Draw() {
    ne::SetCurrentEditor(m_context);
    ne::Begin("My Editor");

    for (const auto& node: m_nodes) {
        node->Draw();
    }

    for (const auto& [linkId, info] : g_links) {
        ne::Link(linkId, info.srcPin, info.dstPin);
    }

    if (ne::BeginCreate()) {
        ne::PinId inputPinId, outputPinId;
        if (ne::QueryNewLink(&inputPinId, &outputPinId)) {
            if (inputPinId && outputPinId) {
                BasePin* pin1 = inputPinId.AsPointer<BasePin>();
                BasePin* pin2 = outputPinId.AsPointer<BasePin>();
                if ((pin1->IsInput() == pin2->IsInput()) || (pin1->GetNode() == pin2->GetNode())) {
                    ne::RejectNewItem();
                } else {
                    auto* src = pin1->IsInput() ? pin2 : pin1;
                    auto* dst = pin1->IsInput() ? pin1 : pin2;
                    bool checkOnly = true;
                    if (!dst->GetNode()->AddIncomingLink(src, dst, checkOnly)) {
                        ne::RejectNewItem();
                    } else if (ne::AcceptNewItem()) { // ne::AcceptNewItem() return true when user release mouse button.
                        checkOnly = false;
                        dst->GetNode()->AddIncomingLink(src, dst, checkOnly);
                        src->GetNode()->AddOutgoingLink(dst->GetNode());

                        auto linkId = ne::LinkId(static_cast<uintptr_t>(g_NextLinkId++));
                        g_links[linkId] = LinkInfo{ne::PinId(src), ne::PinId(dst)};
                        ne::Link(linkId, ne::PinId(src), ne::PinId(dst));
                    }
                }
            } else {
                ne::RejectNewItem();
            }
        }
        ne::EndCreate();
    }

    if (ne::BeginDelete()) {
        ne::LinkId deletedLinkId;
        while (ne::QueryDeletedLink(&deletedLinkId)) {
            const auto it = g_links.find(deletedLinkId);
            if (it == g_links.cend()) {
                ne::RejectDeletedItem();
            } else  if (ne::AcceptDeletedItem()) {
                auto* src = it->second.srcPin.AsPointer<BasePin>();
                auto* dst = it->second.dstPin.AsPointer<BasePin>();
                dst->GetNode()->DelIncomingLink(src, dst);
                src->GetNode()->DelOutgoingLink(dst->GetNode());

                g_links.erase(it);
            }
        }
        ne::EndDelete();
    }

    ne::End();
}

void UINodeEditor::Destroy() {
    if (m_context) {
        m_nodes.clear();
        ne::DestroyEditor(m_context);
        m_context = nullptr;
    }
}
