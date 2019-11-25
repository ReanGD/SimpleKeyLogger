#pragma once

#include <memory>
#include <unordered_map>
#include <imgui_node_editor.h>

#include "middleware/node_editor/base_editor_node.h"


namespace std {
    template <> struct hash<ax::NodeEditor::LinkId> {
        size_t operator()(const ax::NodeEditor::LinkId& k) const {
            return size_t(k);
        }
    };
}

struct LinkInfo {
        ax::NodeEditor::PinId srcPin;
        ax::NodeEditor::PinId dstPin;
};

class NodeEditorStorage : Noncopyable {
public:
    NodeEditorStorage() = default;
    ~NodeEditorStorage();

    void AddNode(const std::shared_ptr<BaseNode>& node);
    bool AddLink(const ax::NodeEditor::PinId pinIdFirst, const ax::NodeEditor::PinId pinIdSecond, bool checkOnly);

    bool DelLink(const ax::NodeEditor::LinkId linkId, bool checkOnly);

    std::shared_ptr<BaseNode> GetNode(const ax::NodeEditor::NodeId nodeId);

    void Draw();

private:
    uintptr_t m_nextId = 1;
    std::vector<std::shared_ptr<BaseNode>> m_nodes;
    std::unordered_map<ax::NodeEditor::LinkId, LinkInfo> m_links;
};
