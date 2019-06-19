#include "editor/editor.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh/mesh.h"
#include "editor/ui_interface.h"
#include "mesh/geometry_generator.h"


std::string Editor::Run() {
    std::string err;
    if (!m_window.Init(false, 0.8f, err)) {
        return err;
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return "Failed to initialize GLEW";
    }

    UIInterface interface;
    if (!m_gui.Init(err)) {
        return err;
    }
    m_gui.EnableInput(m_editorMode);

    glEnable(GL_DEPTH_TEST);

    auto [shaderTex, sErr] = Shader::Create("vertex", "fragment_tex");
    if (!sErr.empty()) {
        return sErr;
    }

    auto [shaderClrLight, slErr] = Shader::Create("vertex", "fragment_clr_light");
    if (!slErr.empty()) {
        return slErr;
    }

    Texture texture;
    if(!texture.Load("brice.jpg", err)) {
        return err;
    }

    Material materialTex(shaderTex);
    materialTex.SetBaseTexture(0, texture);
    Material materialClrLight(shaderClrLight);
    materialClrLight.SetBaseColor(glm::vec3(0.6f, 0.1f, 0.1f));

    Mesh plane, cube, sphere;
    Geometry gmtrPlane = GeometryGenerator::CreateSolidPlane(2, 2, 1.0f, 1.0f);
    plane.Add(gmtrPlane, materialTex);
    Geometry gmtrCube = GeometryGenerator::CreateSolidCube();
    cube.Add(gmtrCube, materialTex);
    Geometry gmtrSphere = GeometryGenerator::CreateSolidSphere(30);
    sphere.Add(gmtrSphere, materialClrLight);

    auto camera = std::make_shared<Camera>(glm::quarter_pi<float>(), 0.1f, 100.0);
    camera->SetViewParams(glm::vec3(-10, 2, 0), glm::vec3(1, 0, 0));

    m_controller.AttachCamera(camera);

    auto timeLast = std::chrono::steady_clock::now();
    while (m_window.StartFrame()) {
        WindowInput& wio = m_window.GetIO();
        ProcessIO();

        uint32_t width, height;
        if (wio.GetFramebufferSize(width, height)) {
            m_controller.SetScreenSize(width, height);
            glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
        }

        auto now = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(now - timeLast).count();
        m_controller.Update(deltaTime);
        m_gui.Update(m_window, deltaTime);
        timeLast = now;

        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto matWorld = glm::mat4(1.0f);
        matWorld = glm::translate(matWorld, glm::vec3(0, 0.51f, 0));
        cube.Draw(camera, matWorld);

        matWorld = glm::scale(glm::mat4(1.0), glm::vec3(20.0f));
        plane.Draw(camera, matWorld);

        matWorld = glm::translate(glm::mat4(1.0), glm::vec3(0, 0.51f, 3.0f));
        matWorld = glm::scale(matWorld, glm::vec3(1.0f, 1.0f, 2.0f));
        sphere.Draw(camera, matWorld);

        interface.Draw(m_gui);

        m_window.EndFrame();
    }

    texture.Delete();
    shaderTex.Delete();
    shaderClrLight.Delete();
    gmtrSphere.Delete();
    gmtrCube.Delete();
    gmtrPlane.Delete();

    return std::string();
}

void Editor::ProcessIO() {
    WindowInput& wio = m_window.GetIO();

    if (wio.IsKeyReleasedFirstTime(Key::Escape)) {
        m_window.Close();
    }

    if (wio.IsKeyReleasedFirstTime(Key::F11)) {
        m_window.SetFullscreen(!m_window.IsFullscreen());
    }

    if (wio.IsKeyReleasedFirstTime(Key::F2)) {
        m_editorMode = !m_editorMode;
        m_window.SetCursor(m_editorMode ? CursorType::Arrow : CursorType::Disabled);
        m_gui.EnableInput(m_editorMode);
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
}