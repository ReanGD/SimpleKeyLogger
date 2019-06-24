#pragma once

#include "engine/engine.h"
#include "engine/mesh/mesh.h"
#include "editor/ui_interface.h"
#include "engine/material/uniform_buffer.h"
#include "engine/camera/fp_camera_control.h"


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
    int m_offset[2];
    std::shared_ptr<UniformBuffer> m_ubCamera;
    UIInterface m_interface;
    FPCameraControl m_controller;
    std::shared_ptr<Camera> m_camera;
};
