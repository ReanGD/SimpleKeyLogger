#pragma once

#include <memory>

#include "engine/common/noncopyable.h"


namespace ax {
    namespace NodeEditor {
        struct EditorContext;
    }
}

class NodeEditorStorage;
class NodeEditor : Noncopyable {
public:
    NodeEditor() = delete;
    NodeEditor(const std::string& name);
    ~NodeEditor();

    void Draw();
private:
    std::string m_name;
    ax::NodeEditor::EditorContext* m_context = nullptr;
    std::shared_ptr<NodeEditorStorage> m_storage = nullptr;
};
