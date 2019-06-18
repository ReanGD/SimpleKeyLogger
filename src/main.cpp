#include <chrono>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh/mesh.h"
#include "editor/ui_interface.h"
#include "mesh/geometry_generator.h"
#include "camera/fp_camera_control.h"


using defer = std::shared_ptr<void>;

std::string run() {
    bool editorMode = false;
    Window window;

    std::string err;
    if (!window.Init(false, 0.8f, err)) {
        return err;
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return "Failed to initialize GLEW";
    }

    Gui gui;
    UIInterface interface;
    if (!gui.Init(err)) {
        return err;
    }
    gui.EnableInput(editorMode);

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

    auto controller = std::make_shared<FPCameraControl>();
    controller->AttachCamera(camera);

    auto timeLast = std::chrono::steady_clock::now();
    while (window.StartFrame()) {
        WindowInput& wio = window.GetIO();

        if (wio.IsKeyReleasedFirstTime(Key::Escape)) {
            window.Close();
        }

        if (wio.IsKeyReleasedFirstTime(Key::F11)) {
            window.SetFullscreen(!window.IsFullscreen());
        }

        if (wio.IsKeyReleasedFirstTime(Key::F2)) {
            editorMode = !editorMode;
            window.SetCursor(editorMode ? CursorType::Arrow : CursorType::Disabled);
            gui.EnableInput(editorMode);
        }

        if (!editorMode) {
            if (wio.IsKeyStickyDown(Key::W)) {
                controller->MoveForward();
            }
            if (wio.IsKeyStickyDown(Key::S)) {
                controller->MoveBackward();
            }
            if (wio.IsKeyStickyDown(Key::A)) {
                controller->MoveLeft();
            }
            if (wio.IsKeyStickyDown(Key::D)) {
                controller->MoveRight();
            }
            float offsetX, offsetY;
            wio.GetCursorOffet(offsetX, offsetY);
            controller->Rotate(offsetX, offsetY);
        }

        uint32_t width, height;
        if (wio.GetFramebufferSize(width, height)) {
            controller->SetScreenSize(width, height);
            glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
        }

        auto now = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(now - timeLast).count();
        controller->Update(deltaTime);
        gui.Update(window, deltaTime);
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

        interface.Draw(gui);

        window.EndFrame();
    }

    texture.Delete();
    shaderTex.Delete();
    shaderClrLight.Delete();
    gmtrSphere.Delete();
    gmtrCube.Delete();
    gmtrPlane.Delete();

    return std::string();
}

void glfwErrorCallback(int error, const char* description) {
    std::cout << "GLFW error (" << error << "): " << description << std::endl;
}

int main() {
    glfwSetErrorCallback(glfwErrorCallback);

    if (glfwInit() != GLFW_TRUE) {
        return EXIT_FAILURE;
    }
    defer _(nullptr, [](...){ glfwTerminate(); });

    try {
        auto err = run();
        if (!err.empty()) {
            std::cerr << "Error: " << err << std::endl;
            return EXIT_FAILURE;
        }
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
