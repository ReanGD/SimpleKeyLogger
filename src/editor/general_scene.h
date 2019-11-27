#pragma once

#include "engine/engine.h"
#include "engine/mesh/scene.h"
#include "middleware/camera/fly_controller.h"


class Shader;
class GeneralScene : Noncopyable {
public:
    GeneralScene() = delete;
    GeneralScene(Engine& engine);
    ~GeneralScene() = default;

private:
    void GenerateGround();
    void GenerateTrees();
    void GenerateGrass();

public:
    void Create();
    void Update(float deltaTime);
    void Draw();
    void Destroy();

private:
    Engine& m_engine;
    Scene m_scene;
    FlyCameraController m_controller;

    std::shared_ptr<Shader> m_shaderTex = nullptr;
    std::shared_ptr<Shader> m_shaderClr = nullptr;
    std::shared_ptr<Shader> m_shaderTexLight = nullptr;
    std::shared_ptr<Shader> m_shaderClrLight = nullptr;
};
