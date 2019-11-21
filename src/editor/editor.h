#pragma once

#include "engine/scene/scene.h"
#include "editor/ui_interface.h"
#include "engine/material/framebuffer.h"
#include "engine/physics/physical_node.h"
#include "engine/material/uniform_buffer.h"
#include "middleware/camera/fly_controller.h"


class Editor : Noncopyable {
public:
    Editor() = delete;
    Editor(Engine& engine);
    ~Editor() = default;


    void Create();
    void Update(float deltaTime);
    void Draw();
    void Destroy();

private:
    void SetEditorMode(bool value);

private:
    Engine& m_engine;
    Scene m_scene;
    bool m_editorMode = false;
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
