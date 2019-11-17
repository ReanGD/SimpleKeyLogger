#pragma once

#include "engine/engine.h"
#include "engine/gui/math.h"
#include "middleware/node_editor/node_editor.h"


struct ImFont;
class Heightmap;
class UINodeEditor;
class UIInterface : Noncopyable {
public:
    UIInterface() = delete;
    UIInterface(Engine& engine);
    ~UIInterface() = default;

    void Init();
    void Render(bool editorMode);
    void Destroy();

private:
    void DrawInfoBar(const math::Rectf& rect);
    void DrawRightPanel(const math::Rectf& rect);
    void DrawViewer(const math::Rectf& rect);
    void DrawNodeEditor(const math::Rectf& rect);
private:
    Engine& m_engine;
    NodeEditor m_editor;
    ImFont* m_fontDefault = nullptr;
    ImFont* m_fontMono = nullptr;
};
