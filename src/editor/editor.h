#pragma once

#include "mesh/mesh.h"
#include "engine/engine.h"
#include "editor/ui_interface.h"
#include "camera/fp_camera_control.h"


class Editor {
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
    void ProcessIO(Engine& engine);

private:
    bool m_editorMode = false;
    Mesh m_cube;
    Mesh m_plane;
    Mesh m_sphere;
    Texture m_texture;
    UIInterface m_interface;
    FPCameraControl m_controller;
    std::shared_ptr<Camera> m_camera;
};
