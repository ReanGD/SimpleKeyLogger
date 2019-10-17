#pragma once

#include <memory>
#include <string>
#include <vector>
#include <imgui_node_editor.h>

#include "engine/common/noncopyable.h"


class BasePin : Noncopyable {
public:
    BasePin() = default;
    ~BasePin() = default;

    void Draw(ne::PinKind direction, bool connected, uint8_t alpha) const noexcept;
};

class BaseNode : Noncopyable {
public:
    BaseNode() = delete;
    BaseNode(const std::string& name);
    ~BaseNode();

    void AddInPin(BasePin* item);
    void AddOutPin(BasePin* item);
    void Draw() const noexcept;

protected:
    std::string m_name;
    std::vector<BasePin*> m_inPins;
    std::vector<BasePin*> m_outPins;
};

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
