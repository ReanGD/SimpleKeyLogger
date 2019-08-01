#pragma once

#include "editor/ui_interface.h"
#include "engine/material/framebuffer.h"
#include "engine/material/uniform_buffer.h"
#include "middleware/camera/fly_controller.h"


class Editor : Noncopyable {
public:
    Editor() = delete;
    Editor(Engine& engine);
    ~Editor() = default;


    bool Init(std::string& error);
    void Render();
    void Destroy();

private:
    void SetEditorMode(bool value);
    void ProcessIO();

private:
    Engine& m_engine;
    bool m_editorMode = false;
    bool m_showNormals = false;
    Texture m_texture;
    Texture m_groundTex;
    Texture m_heightmapTex;
    std::shared_ptr<Framebuffer> m_fbo;
    std::shared_ptr<Material> m_materialLine;
    std::shared_ptr<Material> m_materialNormals;
    std::shared_ptr<Lines> m_line;
    std::shared_ptr<UniformBuffer> m_ubCamera;
    std::shared_ptr<UniformBufferDecl> m_declCamera;
    UIInterface m_interface;
    FlyCameraController m_controller;
};
