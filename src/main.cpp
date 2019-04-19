#include <chrono>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "mesh/mesh_generator.h"
#include "material/shader.h"
#include "material/texture.h"
#include "camera/camera_fps.h"
#include "window/window.h"


using defer = std::shared_ptr<void>;
using error = std::string;


void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

CameraFps cameraFps;
// const GLuint WIDTH = 1024, HEIGHT = 768;
const GLuint WIDTH = 1920, HEIGHT = 1080;


std::string run() {
    Window window(WIDTH, HEIGHT);

    std::string err;
    if (!window.Init(false, err)) {
        return err;
    }

    glfwSetCursorPosCallback(window.m_handle, mouseCallback);
    // glfwSetInputMode(window.m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window.m_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return "Failed to initialize GLEW";
    }

    int width, height;
    glfwGetFramebufferSize(window.m_handle, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    Mesh plane = MeshGenerator::CreateSolidPlane(2, 2, 1.0f, 1.0f);
    Mesh cube = MeshGenerator::CreateSolidCube();
    Mesh sphere = MeshGenerator::CreateSolidSphere(30);

    auto shader = Shader::Create("vertex", "fragment");
    if (!shader) {
        return shader.error();
    }

    auto shaderLight = Shader::Create("vertex_light", "fragment_light");
    if (!shaderLight) {
        return shaderLight.error();
    }

    auto texture = Texture::Create("brice.jpg");
    if (!texture) {
        return texture.error();
    }

    auto camera = std::make_shared<Camera>(glm::quarter_pi<float>(), width, height, 0.1f, 100.0);
    camera->SetViewParams(glm::vec3(-10, 2, 0), glm::vec3(1, 0, 0));
    cameraFps.AttachCamera(camera);

    auto timeLast = std::chrono::steady_clock::now();
    while (!glfwWindowShouldClose(window.m_handle)) {
        auto now = std::chrono::steady_clock::now();
        cameraFps.Update(std::chrono::duration<float>(now - timeLast).count());
        timeLast = now;

        processInput(window.m_handle);

        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Bind();

        glActiveTexture(GL_TEXTURE0);
        texture->Bind();

        shader->SetInt("ourTexture1", 0);

        shader->SetMat4("uProj", camera->GetProjMatrix());
        shader->SetMat4("uView", camera->GetViewMatrix());

        auto matWorld = glm::mat4(1.0f);
        matWorld = glm::translate(matWorld, glm::vec3(0, 0.51f, 0));
        shader->SetMat4("uWorld", matWorld);
        glm::mat3 matNorm = glm::inverseTranspose(glm::mat3(matWorld));
        shader->SetMat3("uNorm", matNorm);

        cube.Bind();
        cube.Draw();
        cube.Unbind();

        matWorld = glm::scale(glm::mat4(1.0), glm::vec3(20.0f));
        shader->SetMat4("uWorld", matWorld);
        matNorm = glm::inverseTranspose(glm::mat3(matWorld));
        shader->SetMat3("uNorm", matNorm);

        plane.Bind();
        plane.Draw();
        plane.Unbind();

        texture->Unbind();
        shader->Unbind();


        shaderLight->Bind();

        glActiveTexture(GL_TEXTURE0);
        texture->Bind();

        shaderLight->SetInt("uTextureDiffuse", 0);

        shaderLight->SetMat4("uProjMatrix", camera->GetProjMatrix());
        shaderLight->SetMat4("uViewMatrix", camera->GetViewMatrix());
        shaderLight->SetVec3("uToEyeDirection", -camera->GetDirection());

        matWorld = glm::mat4(1.0f);
        matWorld = glm::translate(matWorld, glm::vec3(0, 0.51f, 3.0f));
        matWorld = glm::scale(matWorld, glm::vec3(1.0f, 1.0f, 2.0f));
        shaderLight->SetMat4("uWorldMatrix", matWorld);
        matNorm = glm::inverseTranspose(glm::mat3(matWorld));
        shaderLight->SetMat3("uNormalMatrix", matNorm);

        sphere.Bind();
        sphere.Draw();
        sphere.Unbind();

        matWorld = glm::mat4(1.0f);
        matWorld = glm::translate(matWorld, glm::vec3(0, 0.51f, 6.0f));
        matWorld = glm::scale(matWorld, glm::vec3(1.0f, 1.0f, 2.0f));
        shaderLight->SetMat4("uWorldMatrix", matWorld);
        matNorm = glm::mat3(matWorld);
        shaderLight->SetMat3("uNormalMatrix", matNorm);

        sphere.Bind();
        sphere.Draw();
        sphere.Unbind();

        texture->Unbind();
        shaderLight->Unbind();

        glfwSwapBuffers(window.m_handle);
    }

    texture->Delete();
    shader->Delete();
    shaderLight->Delete();
    sphere.Delete();
    cube.Delete();
    plane.Delete();

    return std::string();
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraFps.MoveForward();
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraFps.MoveBackward();
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraFps.MoveLeft();
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraFps.MoveRight();
    }
}

bool firstMouse = true;
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;
void mouseCallback(GLFWwindow* window __attribute__((unused)), double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    cameraFps.Rotate(float(lastX - xpos), float(lastY - ypos));

    lastX = xpos;
    lastY = ypos;
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