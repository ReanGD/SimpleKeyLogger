#include <chrono>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/gtc/matrix_transform.hpp>
#include "mesh/mesh.h"
#include "shader/shader.h"
#include "camera/camera_fps.h"


void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

CameraFps cameraFps;
const GLuint WIDTH = 1024, HEIGHT = 768;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "RTGE", nullptr, nullptr);    
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }    

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);  
    glViewport(0, 0, width, height);

    GLfloat vertices[] = {
        // Positions          // Colors           // Texture Coords
        0.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
        0.0f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        0.0f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        0.0f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    DataBuffer vertexBuffer(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
    DataBuffer indexBuffer(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices));
    Mesh mesh(VDecls::PosColorTex, vertexBuffer, indexBuffer);

    auto shader = Shader::Create("vertex", "fragment");
     if (!shader) {
        std::cout << shader.error() << std::endl;
        return -1;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int texWidth, texHeight, texChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *texData = stbi_load("data/textures/brice.jpg",
                                    &texWidth,
                                    &texHeight,
                                    &texChannels,
                                    STBI_rgb);    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texData);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

    auto camera = std::make_shared<Camera>(glm::quarter_pi<float>(), width, height, 0.1f, 100.0);    
    camera->SetViewParams(glm::vec3(-10, 0, 0), glm::vec3(1, 0, 0));
    cameraFps.AttachCamera(camera);

    auto timeLast = std::chrono::steady_clock::now();
    while (!glfwWindowShouldClose(window)) {
        auto now = std::chrono::steady_clock::now();
        cameraFps.Update(std::chrono::duration<float>(now - timeLast).count());
        timeLast = now;

        processInput(window);

        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->Bind();

        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader->SetInt("ourTexture1", 0);

        glm::mat4 matWorld = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        matWorld = glm::translate(matWorld, glm::vec3(0.5f, -0.5f, 0.0f));
        matWorld = glm::rotate(matWorld, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        shader->SetMat4("matProj", camera->GetProjMatrix());
        shader->SetMat4("matView", camera->GetViewMatrix());
        shader->SetMat4("matWorld", matWorld);
        
        mesh.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        mesh.Unbind();

        shader->Unbind();

        glfwSwapBuffers(window);
    }

    shader->Delete();
    mesh.Delete();

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
