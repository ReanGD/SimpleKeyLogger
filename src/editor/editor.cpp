#include "editor/editor.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <fmt/format.h>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/mesh/geometry_generator.h"


bool Editor::Init(Engine& engine, std::string& error) {
    SetEditorMode(engine, m_editorMode);

    m_camera = std::make_shared<Camera>(glm::quarter_pi<float>(), 0.1f, 100.0);
    m_camera->SetViewParams(glm::vec3(-10, 2, 0), glm::vec3(1, 0, 0));

    m_controller.AttachCamera(m_camera);

    auto shaderTex = Shader::Create("vertex_old", "fragment_tex", error);
    if (!shaderTex) {
        return false;
    }

    auto shaderTexLight = Shader::Create("vertex", "fragment_tex_light", error);
    if (!shaderTexLight) {
        return false;
    }

    m_declCamera = shaderTexLight->GetUBDecl("ubCamera");
    if (!m_declCamera) {
        error = fmt::format("Сouldn't found uniform block '{}' in shader", "ubCamera");
        return false;
    }

    m_ubCamera = std::make_shared<UniformBuffer>(m_declCamera->GetSize());
    m_ubCamera->setUniform(m_declCamera->GetOffset("uProjMatrix"), m_camera->GetProjMatrix());
    m_ubCamera->setUniform(m_declCamera->GetOffset("uViewMatrix"), m_camera->GetViewMatrix());
    m_ubCamera->Sync();

    GLuint index = 0;
    glUniformBlockBinding(shaderTexLight->GetHandle(), m_declCamera->GetIndex(), index);
    m_ubCamera->Bind(index);

    auto shaderClrLight = Shader::Create("vertex_old", "fragment_clr_light", error);
    if (!shaderClrLight) {
        return false;
    }

    if(!m_texture.Load("brice.jpg", error)) {
        return false;
    }
    if(!m_groundTex.Load("ground.jpg", error)) {
        return false;
    }

    Material materialTex(shaderTex);
    materialTex.SetBaseTexture(0, m_texture);
    Material materialGround(shaderTex);
    materialGround.SetBaseTexture(0, m_groundTex);

    m_cube.Add(GeometryGenerator::CreateSolidCube(), materialTex);
    m_plane.Add(GeometryGenerator::CreateSolidPlane(2, 2, 4.0f, 4.0f), materialGround);

    Material materialSphere(shaderTexLight);
    materialSphere.SetBaseColor(glm::vec3(0.6f, 0.1f, 0.1f));
    materialSphere.SetBaseTexture(0, m_groundTex);
    auto sphereGeom = GeometryGenerator::CreateSolidSphere(100);
    for(auto& meshesRow: m_meshes) {
        for(auto& mesh: meshesRow) {
            mesh.Add(sphereGeom, materialSphere);
        }
    }

    return true;
}

void Editor::Render(Engine& engine) {
    ProcessIO(engine);

    m_ubCamera->setUniform(m_declCamera->GetOffset("uProjMatrix"), m_camera->GetProjMatrix());
    m_ubCamera->setUniform(m_declCamera->GetOffset("uViewMatrix"), m_camera->GetViewMatrix());
    m_ubCamera->Sync();

    GLuint index = 0;
    m_ubCamera->Bind(index);

    auto matWorld = glm::mat4(1.0f);
    matWorld = glm::translate(matWorld, glm::vec3(3.0, 0.51f, 0));
    m_cube.Draw(m_camera, matWorld);

    matWorld = glm::scale(glm::mat4(1.0), glm::vec3(40.0f));
    m_plane.Draw(m_camera, matWorld);

    float posX = -static_cast<float>(m_meshes.size()) * 1.5f / 2.0f;
    for(const auto& meshesRow: m_meshes) {
        float posZ = -static_cast<float>(m_meshes[0].size()) * 2.5f / 2.0f;
        for(const auto& mesh: meshesRow) {
            matWorld = glm::translate(glm::mat4(1.0), glm::vec3(posX, 0.51f, posZ));
            posZ += 2.5f;
            matWorld = glm::scale(matWorld, glm::vec3(1.0f, 1.0f, 2.0f));
            mesh.Draw(m_camera, matWorld);
        }
        posX += 1.5f;
    }

    m_interface.Draw(engine.GetGui());
}

void Editor::Destroy() {
    m_texture.Destroy();
    m_groundTex.Destroy();
}

void Editor::SetEditorMode(Engine& engine, bool value) {
    m_editorMode = value;
    engine.GetWindow().SetCursor(m_editorMode ? CursorType::Arrow : CursorType::Disabled);
    engine.GetGui().EnableInput(m_editorMode);
    m_controller.EnableInput(!m_editorMode);
}

void Editor::ProcessIO(Engine& engine) {
    auto& window = engine.GetWindow();
    WindowInput& wio = window.GetIO();

    if (wio.IsKeyReleasedFirstTime(Key::Escape)) {
        window.Close();
    }

    if (wio.IsKeyReleasedFirstTime(Key::F11)) {
        window.SetFullscreen(!window.IsFullscreen());
    }

    if (wio.IsKeyReleasedFirstTime(Key::F2)) {
        SetEditorMode(engine, !m_editorMode);
    }

    m_controller.Update(wio, engine.GetDeltaTime());
}
