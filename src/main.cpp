#include <chrono>
#include <iostream>

#include <imgui.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "gui/gui.h"
#include "mesh/mesh.h"
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
    if (!gui.Init(err)) {
        return err;
    }
    gui.EnableInput(editorMode);

    glEnable(GL_DEPTH_TEST);

    auto [shader, sErr] = Shader::Create("vertex", "fragment");
    if (!sErr.empty()) {
        return sErr;
    }

    auto [shaderLight, slErr] = Shader::Create("vertex", "fragment_light");
    if (!slErr.empty()) {
        return slErr;
    }

    Texture texture;
    if(!texture.Load("brice.jpg", err)) {
        return err;
    }

    Material material(shader);
    material.SetBaseTexture(0, texture);
    Material materialLight(shaderLight);
    materialLight.SetBaseTexture(0, texture);

    Geometry gmtrPlane = GeometryGenerator::CreateSolidPlane(2, 2, 1.0f, 1.0f);
    Geometry gmtrCube = GeometryGenerator::CreateSolidCube();
    Geometry gmtrSphere = GeometryGenerator::CreateSolidSphere(30);
    Mesh plane, cube, sphere;
    plane.Add(gmtrPlane, material);
    cube.Add(gmtrCube, material);
    sphere.Add(gmtrSphere, materialLight);

    auto camera = std::make_shared<Camera>(glm::quarter_pi<float>(), 0.1f, 100.0);
    camera->SetViewParams(glm::vec3(-10, 2, 0), glm::vec3(1, 0, 0));

    auto controller = std::make_shared<FPCameraControl>();
    controller->AttachCamera(camera);

    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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

        gui.NewFrame();
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Win1");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&clear_color)); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        gui.EndFrame();

        window.EndFrame();
    }

    texture.Delete();
    shader.Delete();
    shaderLight.Delete();
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
