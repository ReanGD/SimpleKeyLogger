#include "editor/ui_node_editor.h"

#include <imgui_node_editor.h>
#include "middleware/node_editor/noise_node.h"


namespace ne = ax::NodeEditor;

struct LinkInfo {
    ne::LinkId Id;
    ne::PinId InputId;
    ne::PinId OutputId;
};

static ImVector<LinkInfo>   g_Links;
static int                  g_NextLinkId = 100;


bool UINodeEditor::Create(std::string& error) {
    Destroy();
    m_context = ne::CreateEditor();

    auto perlinNode = std::make_shared<PerlinNode>();
    if (!perlinNode->Create(error)) {
        return false;
    }
    m_nodes.push_back(perlinNode);

    auto node1 = std::make_shared<BaseNode>("Perlin noise1");
    node1->AddInPin(new BasePin());
    node1->AddInPin(new BasePin());
    node1->AddOutPin(new BasePin());

    m_nodes.push_back(node1);

    auto node2 = std::make_shared<BaseNode>("Perlin noise2");
    node2->AddInPin(new BasePin());
    node2->AddInPin(new BasePin());
    node2->AddOutPin(new BasePin());

    m_nodes.push_back(node2);
    return true;
}

void UINodeEditor::Draw() {
    ne::SetCurrentEditor(m_context);
    ne::Begin("My Editor");

    for (const auto& node: m_nodes) {
        node->Draw();
    }

    // Submit Links
    for (auto& linkInfo : g_Links) {
        ne::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);
    }

    // Handle creation action, returns true if editor want to create new object (node or link)
    if (ne::BeginCreate()) {
        ne::PinId inputPinId, outputPinId;
        if (ne::QueryNewLink(&inputPinId, &outputPinId)) {
            // QueryNewLink returns true if editor want to create new link between pins.
            //
            // Link can be created only for two valid pins, it is up to you to
            // validate if connection make sense. Editor is happy to make any.
            //
            // Link always goes from input to output. User may choose to drag
            // link from output pin or input pin. This determine which pin ids
            // are valid and which are not:
            //   * input valid, output invalid - user started to drag new ling from input pin
            //   * input invalid, output valid - user started to drag new ling from output pin
            //   * input valid, output valid   - user dragged link over other pin, can be validated

            // both are valid, let's accept link
            if (inputPinId && outputPinId) {
                // ne::AcceptNewItem() return true when user release mouse button.
                if (ne::AcceptNewItem()) {
                    // Since we accepted new link, lets add one to our list of links.
                    g_Links.push_back({ ne::LinkId(static_cast<uintptr_t>(g_NextLinkId++)), inputPinId, outputPinId });

                    // Draw new link.
                    ne::Link(g_Links.back().Id, g_Links.back().InputId, g_Links.back().OutputId);
                }

                // You may choose to reject connection between these nodes
                // by calling ne::RejectNewItem(). This will allow editor to give
                // visual feedback by changing link thickness and color.
            }
        }
        ne::EndCreate();
    }

    if (ne::BeginDelete()) {
        // There may be many links marked for deletion, let's loop over them.
        ne::LinkId deletedLinkId;
        while (ne::QueryDeletedLink(&deletedLinkId)) {
            // If you agree that link can be deleted, accept deletion.
            if (ne::AcceptDeletedItem()) {
                // Then remove link from your data.
                for (auto& link : g_Links) {
                    if (link.Id == deletedLinkId) {
                        g_Links.erase(&link);
                        break;
                    }
                }
            }

            // You may reject link deletion by calling:
            // ne::RejectDeletedItem();
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
