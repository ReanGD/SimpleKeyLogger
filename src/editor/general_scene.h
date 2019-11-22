#pragma once

#include "engine/engine.h"
#include "engine/scene/scene.h"
#include "middleware/camera/fly_controller.h"


class GeneralScene : Noncopyable {
public:
    GeneralScene() = delete;
    GeneralScene(Engine& engine);
    ~GeneralScene() = default;

    void Create();
    void Update(float deltaTime);
    void Draw();
    void Destroy();

private:
    Engine& m_engine;
    Scene m_scene;
    FlyCameraController m_controller;
};
