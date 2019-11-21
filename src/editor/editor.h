#pragma once

#include "editor/ui_interface.h"
#include "engine/material/framebuffer.h"
#include "engine/material/uniform_buffer.h"
#include "middleware/camera/fly_controller.h"
#include "engine/physics/physical_node.h"


class Editor : Noncopyable {
public:
    Editor() = delete;
    Editor(Engine& engine);
    ~Editor() = default;


    void Create();
    void Draw();
    void Destroy();

private:
    void SetEditorMode(bool value);
    void ProcessIO();

private:
    Engine& m_engine;
    bool m_editorMode = true;
    bool m_showNormals = false;
    std::shared_ptr<Material> m_materialCube;
    std::shared_ptr<Framebuffer> m_fbo;
    std::shared_ptr<Material> m_materialLine;
    std::shared_ptr<Material> m_materialNormals;
    std::shared_ptr<Lines> m_line;
    std::shared_ptr<UniformBuffer> m_ubCamera;
    std::shared_ptr<UniformBufferDecl> m_declCamera;
    UIInterface m_interface;
    FlyCameraController m_controller;
};
