#pragma once

#include "engine/engine.h"
#include "editor/ui_interface.h"
#include "engine/material/uniform_buffer.h"
#include "middleware/camera/fly_controller.h"


class Editor : Noncopyable {
public:
    Editor(const Editor&) = delete;
    Editor(Editor&&) = delete;
    Editor& operator=(const Editor&) = delete;
    Editor& operator=(Editor&&) = delete;

    Editor() = default;
    ~Editor() = default;


    bool Init(Engine& engine, std::string& error);
    void Render(Engine& engine);
    void Destroy();

private:
    void SetEditorMode(Engine& engine, bool value);
    void ProcessIO(Engine& engine);

private:
    bool m_editorMode = false;
    Texture m_texture;
    Texture m_groundTex;
    Texture m_heightmapTex;
    std::shared_ptr<Material> m_materialLine;
    std::shared_ptr<Lines> m_line;
    std::shared_ptr<UniformBuffer> m_ubCamera;
    std::shared_ptr<UniformBufferDecl> m_declCamera;
    UIInterface m_interface;
    FlyCameraController m_controller;
};
