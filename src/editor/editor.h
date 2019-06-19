#pragma once

#include "gui/gui.h"
#include "camera/fp_camera_control.h"


class Editor {
public:
    Editor(const Editor&) = delete;
    Editor(Editor&&) = delete;
    Editor& operator=(const Editor&) = delete;
    Editor& operator=(Editor&&) = delete;

    Editor() = default;
    ~Editor() = default;

    std::string Run();

private:
    void ProcessIO();

private:
    bool m_editorMode = false;
    Gui m_gui;
    Window m_window;
    FPCameraControl m_controller;
};
