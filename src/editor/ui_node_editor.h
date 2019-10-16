#pragma once

#include <string>
#include <imgui_node_editor.h>

#include "engine/common/noncopyable.h"


class UINodeEditor : Noncopyable {
public:
    UINodeEditor() = default;
    ~UINodeEditor() = default;

    bool Create(std::string& error);
    void Draw();
    void Destroy();

private:
    ax::NodeEditor::EditorContext* m_context = nullptr;
};
