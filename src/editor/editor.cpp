#include "editor/editor.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include "mesh/geometry_generator.h"

bool Editor::Init(Engine& engine, std::string& error) {
    auto& gui = engine.GetGui();
    gui.EnableInput(m_editorMode);

    auto shaderTex = Shader::Create("vertex", "fragment_tex", error);
    if (!shaderTex) {
        return false;
    }

    auto shaderClrLight = Shader::Create("vertex", "fragment_clr_light", error);
    if (!shaderClrLight) {
        return false;
    }

    if(!m_texture.Load("brice.jpg", error)) {
        return false;
    }

    Material materialTex(shaderTex);
    materialTex.SetBaseTexture(0, m_texture);
    Material materialClrLight(shaderClrLight);
    materialClrLight.SetBaseColor(glm::vec3(0.6f, 0.1f, 0.1f));

    m_cube.Add(GeometryGenerator::CreateSolidCube(), materialTex);
    m_plane.Add(GeometryGenerator::CreateSolidPlane(2, 2, 1.0f, 1.0f), materialTex);
    m_sphere.Add(GeometryGenerator::CreateSolidSphere(30), materialClrLight);

    m_camera = std::make_shared<Camera>(glm::quarter_pi<float>(), 0.1f, 100.0);
    m_camera->SetViewParams(glm::vec3(-10, 2, 0), glm::vec3(1, 0, 0));

    m_controller.AttachCamera(m_camera);

    return true;
}

void Editor::Render(Engine& engine) {
    ProcessIO(engine);

    auto matWorld = glm::mat4(1.0f);
    matWorld = glm::translate(matWorld, glm::vec3(0, 0.51f, 0));
    m_cube.Draw(m_camera, matWorld);

    matWorld = glm::scale(glm::mat4(1.0), glm::vec3(20.0f));
    m_plane.Draw(m_camera, matWorld);

    matWorld = glm::translate(glm::mat4(1.0), glm::vec3(0, 0.51f, 3.0f));
    matWorld = glm::scale(matWorld, glm::vec3(1.0f, 1.0f, 2.0f));
    m_sphere.Draw(m_camera, matWorld);

    m_interface.Draw(engine.GetGui());
}

void Editor::Destroy() {
    m_texture.Destroy();
}

void Editor::ProcessIO(Engine& engine) {
    auto& window = engine.GetWindow();
    auto& gui = engine.GetGui();
    WindowInput& wio = window.GetIO();

    uint32_t width, height;
    if (wio.GetFramebufferSize(width, height)) {
        m_controller.SetScreenSize(width, height);
    }

    if (wio.IsKeyReleasedFirstTime(Key::Escape)) {
        window.Close();
    }

    if (wio.IsKeyReleasedFirstTime(Key::F11)) {
        window.SetFullscreen(!window.IsFullscreen());
    }

    if (wio.IsKeyReleasedFirstTime(Key::F2)) {
        m_editorMode = !m_editorMode;
        window.SetCursor(m_editorMode ? CursorType::Arrow : CursorType::Disabled);
        gui.EnableInput(m_editorMode);
    }

    if (!m_editorMode) {
        if (wio.IsKeyStickyDown(Key::W)) {
            m_controller.MoveForward();
        }
        if (wio.IsKeyStickyDown(Key::S)) {
            m_controller.MoveBackward();
        }
        if (wio.IsKeyStickyDown(Key::A)) {
            m_controller.MoveLeft();
        }
        if (wio.IsKeyStickyDown(Key::D)) {
            m_controller.MoveRight();
        }
        float offsetX, offsetY;
        wio.GetCursorOffet(offsetX, offsetY);
        m_controller.Rotate(offsetX, offsetY);
    }

    m_controller.Update(engine.GetDeltaTime());
}
