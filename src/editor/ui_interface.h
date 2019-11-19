#pragma once

#include "engine/engine.h"
#include "engine/gui/math.h"
#include "middleware/node_editor/node_editor.h"


struct ImFont;
class UINodeEditor;
class UIInterface : Noncopyable {
public:
    UIInterface() = delete;
    UIInterface(Engine& engine);
    ~UIInterface() = default;

    void Init();
    void Render(bool editorMode, const std::shared_ptr<Texture>& texture);
    void Destroy();

private:
    void DrawInfoBar(math::Rectf rect);
    void DrawRightPanel(math::Rectf rect);
    void DrawViewer(math::Rectf rect, const std::shared_ptr<Texture>& texture);
    void DrawNodeEditor(math::Rectf rect);
private:
    Engine& m_engine;
    NodeEditor m_editor;
    ImFont* m_fontDefault = nullptr;
    ImFont* m_fontMono = nullptr;
};
