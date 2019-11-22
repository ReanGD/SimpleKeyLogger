#pragma once

#include "editor/ui_interface.h"
#include "editor/general_scene.h"


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
    GeneralScene m_generalScene;
    bool m_editorMode = false;
    UIInterface m_interface;
};
