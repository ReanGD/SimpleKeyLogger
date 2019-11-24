#include "editor/general_scene.h"

#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/material/texture_manager.h"
#include "middleware/generator/mesh_generator.h"


static constexpr const auto one = glm::mat4(1);

GeneralScene::GeneralScene(Engine& engine)
    : m_engine(engine) {

}

void GeneralScene::GenerateGround() {
    auto& texManager = TextureManager::Get();
    auto groundTex = texManager.Load("$tex/ground.jpg");

    Material materialGround(m_shaderTex);
    materialGround.SetBaseTexture(0, groundTex);

    Node plane;
    plane.Add(MeshGenerator::CreateSolidPlane(2, 2, 4.0f, 4.0f), materialGround);
    auto matModel = glm::scale(one, glm::vec3(256, 1, 256));
    plane.SetModelMatrix(matModel);
    m_scene.Add(plane);
}

void GeneralScene::GenerateTrees() {
    Material materialTreeTrunk(m_shaderClrLight);
    materialTreeTrunk.SetBaseColor(glm::vec3(139, 69, 19) / 255.0f);
    Material materialTreeCrown(m_shaderClrLight);
    materialTreeCrown.SetBaseColor(glm::vec3(0, 128, 0) / 255.0f);


    auto trunk = MeshGenerator::CreateSolidCylinder(5);
    const glm::mat4 matModelTrunk = glm::translate(one, glm::vec3(0, 2, 0)) * glm::scale(one, glm::vec3(0.5, 4, 0.5));

    auto crown = MeshGenerator::CreateSolidSphere(10);
    const glm::mat4 matModelCrown = glm::translate(one, glm::vec3(0, 7, 0)) * glm::scale(one, glm::vec3(4, 8, 4));

    std::srand(5);
    for (auto i=0; i!=100; ++i) {
        auto matModelPosition = glm::translate(one, glm::linearRand(glm::vec3(-100, 0, -100), glm::vec3(100, 0, 100)));

        Node treeTrunk;
        treeTrunk.Add(trunk, materialTreeTrunk);
        treeTrunk.SetModelMatrix(matModelPosition * matModelTrunk);
        m_scene.Add(treeTrunk);

        Node treeCrown;
        treeCrown.Add(crown, materialTreeCrown);
        treeCrown.SetModelMatrix(matModelPosition * matModelCrown);
        m_scene.Add(treeCrown);
    }
}

void GeneralScene::Create() {
    auto camera = m_scene.GetCamera();
    camera->SetViewParams(glm::vec3(-10, 2, 0), glm::vec3(1, 0, 0));
    m_controller.AttachCamera(camera);

    m_shaderTex = Shader::Create("vertex_old", "fragment_tex");
    m_shaderClr = Shader::Create("vertex_old", "fragment_clr");
    m_shaderTexLight = Shader::Create("vertex_old", "fragment_tex_light");
    m_shaderClrLight = Shader::Create("vertex_old", "fragment_clr_light");

    GenerateGround();
    GenerateTrees();
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
