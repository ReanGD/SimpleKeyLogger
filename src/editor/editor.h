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
    std::array<std::array<Mesh, 100>, 100> m_meshes;
    Mesh m_cube;
    Mesh m_plane;
    Texture m_texture;
    Texture m_groundTex;
    UIInterface m_interface;
    FPCameraControl m_controller;
    std::shared_ptr<Camera> m_camera;
};
