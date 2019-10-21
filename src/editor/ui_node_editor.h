#pragma once

#include <memory>
#include <string>
#include <vector>

#include "engine/common/noncopyable.h"


namespace ax {
    namespace NodeEditor {
        struct EditorContext;
    }
}

class BaseNode;
class UINodeEditor : Noncopyable {
public:
    UINodeEditor() = default;
    ~UINodeEditor() = default;

    bool Create(std::string& error);
    void Draw();
    void Destroy();

private:
    ax::NodeEditor::EditorContext* m_context = nullptr;
    std::vector<std::shared_ptr<BaseNode>> m_nodes;
};
