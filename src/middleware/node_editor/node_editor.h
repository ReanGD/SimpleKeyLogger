#pragma once

#include <memory>

#include "engine/common/noncopyable.h"


namespace ax {
    namespace NodeEditor {
        struct EditorContext;
    }
}

class BaseNode;
class NodeEditorStorage;
class NodeEditor : Noncopyable {
public:
    NodeEditor() = delete;
    NodeEditor(const std::string& name);
    ~NodeEditor();

    std::shared_ptr<BaseNode> GetViewNode() noexcept { return m_viewNode; }

    void Draw();
private:
    std::shared_ptr<BaseNode> EditorMenu();

private:
    std::string m_name;
    ax::NodeEditor::EditorContext* m_context = nullptr;
    std::shared_ptr<NodeEditorStorage> m_storage = nullptr;
    std::shared_ptr<BaseNode> m_viewNode = nullptr;
};
