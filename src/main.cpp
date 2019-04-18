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


void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

CameraFps cameraFps;
const GLuint WIDTH = 1024, HEIGHT = 768;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "RTGE", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    Mesh plane = MeshGenerator::CreateSolidPlane(2, 2, 1.0f, 1.0f);
    Mesh cube = MeshGenerator::CreateSolidCube();
    Mesh sphere = MeshGenerator::CreateSolidSphere(30);

    auto shader = Shader::Create("vertex", "fragment");
    if (!shader) {
        std::cout << shader.error() << std::endl;
        glfwTerminate();
        return -1;
    }

    auto shaderLight = Shader::Create("vertex_light", "fragment_light");
    if (!shaderLight) {
        std::cout << shaderLight.error() << std::endl;
        glfwTerminate();
        return -1;
    }

    auto texture = Texture::Create("brice.jpg");
    if (!texture) {
        std::cout << texture.error() << std::endl;
        glfwTerminate();
        return -1;
    }

    auto camera = std::make_shared<Camera>(glm::quarter_pi<float>(), width, height, 0.1f, 100.0);
    camera->SetViewParams(glm::vec3(-10, 2, 0), glm::vec3(1, 0, 0));
    cameraFps.AttachCamera(camera);

    auto timeLast = std::chrono::steady_clock::now();
    while (!glfwWindowShouldClose(window)) {
        auto now = std::chrono::steady_clock::now();
        cameraFps.Update(std::chrono::duration<float>(now - timeLast).count());
        timeLast = now;

        processInput(window);

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

        glfwSwapBuffers(window);
    }

    texture->Delete();
    shader->Delete();
    shaderLight->Delete();
    sphere.Delete();
    cube.Delete();
    plane.Delete();

    glfwTerminate();
    return 0;
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
