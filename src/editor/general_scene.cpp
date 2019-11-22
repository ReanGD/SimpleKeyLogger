#include "editor/general_scene.h"

#include <glm/gtc/matrix_transform.hpp>

#include "engine/mesh/geometry_generator.h"
#include "engine/material/texture_manager.h"


GeneralScene::GeneralScene(Engine& engine)
    : m_engine(engine) {

}

void GeneralScene::Create() {
    auto camera = m_scene.GetCamera();
    camera->SetViewParams(glm::vec3(-10, 2, 0), glm::vec3(1, 0, 0));
    m_controller.AttachCamera(camera);

    auto shaderTex = Shader::Create("vertex_old", "fragment_tex");
    auto shaderClr = Shader::Create("vertex_old", "fragment_clr");
    auto shaderTexLight = Shader::Create("vertex_old", "fragment_tex_light");
    auto shaderClrLight = Shader::Create("vertex_old", "fragment_clr_light");

    auto& texManager = TextureManager::Get();
    auto groundTex = texManager.Load("$tex/ground.jpg");

    Material materialGround(shaderTex);
    materialGround.SetBaseTexture(0, groundTex);

    Mesh plane;
    plane.Add(GeometryGenerator::CreateSolidPlane(2, 2, 4.0f, 4.0f), materialGround);
    auto matModel = glm::scale(glm::mat4(1.0), glm::vec3(256., 1.0f, 256));
    plane.SetModelMatrix(matModel);
    m_scene.Add(plane);

}

void GeneralScene::Update(float deltaTime) {
    WindowInput& wio = m_engine.GetWindow().GetIO();

    m_scene.Update();
    m_controller.Update(wio, deltaTime);
}

void GeneralScene::Draw() {
    m_scene.Draw();
}

void GeneralScene::Destroy() {

}