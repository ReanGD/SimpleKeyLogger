#pragma once

#include "engine/engine.h"

struct rect {
    uint32_t posX = 0;
    uint32_t posY = 0;
    uint32_t sizeX = 0;
    uint32_t sizeY = 0;
};

struct ImFont;
class Heightmap;
class UINodeEditor;
class UIInterface : Noncopyable {
public:
    UIInterface() = delete;
    UIInterface(Engine& engine);
    ~UIInterface() = default;

    bool Init(std::string& error);
    void Render(bool editorMode);
    void Destroy();

private:
    void DrawInfoBar(rect& rect);
    void DrawRightPanel(rect& rect);
    void DrawViewer(rect& rect);
    void DrawNodeEditor(rect& rect);
private:
    Engine& m_engine;
    std::shared_ptr<UINodeEditor> m_nodeEditor = nullptr;
    ImFont* m_fontDefault = nullptr;
    ImFont* m_fontMono = nullptr;
    std::shared_ptr<Heightmap> m_heightmap;
};
