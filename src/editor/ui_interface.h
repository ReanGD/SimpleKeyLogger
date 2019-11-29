#pragma once

#include "engine/engine.h"
#include "engine/common/math.h"
#include "middleware/node_editor/node_editor.h"


struct ImFont;
class Camera;
class UINodeEditor;
class UIInterface : Noncopyable {
public:
    UIInterface() = delete;
    UIInterface(Engine& engine);
    ~UIInterface() = default;

    void Create();
    void Draw(bool editorMode, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture>& texture, uint32_t tpf);
    void Destroy();

private:
    void DrawInfoBar(math::Rectf rect, const std::shared_ptr<Camera>& camera, uint32_t tpf);
    void DrawRightPanel(math::Rectf rect, const std::shared_ptr<Camera>& camera);
    void DrawViewer(math::Rectf rect, const std::shared_ptr<Texture>& texture);
    void DrawNodeEditor(math::Rectf rect);
private:
    Engine& m_engine;
    NodeEditor m_editor;
    ImFont* m_fontDefault = nullptr;
    ImFont* m_fontMono = nullptr;
};
