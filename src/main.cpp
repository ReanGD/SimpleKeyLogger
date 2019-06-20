#include <iostream>
#include <GLFW/glfw3.h>
#include "editor/editor.h"


void glfwErrorCallback(int error, const char* description) {
    std::cout << "GLFW error (" << error << "): " << description << std::endl;
}

int main() {
    glfwSetErrorCallback(glfwErrorCallback);

    if (glfwInit() != GLFW_TRUE) {
        return EXIT_FAILURE;
    }
    std::shared_ptr<void> _(nullptr, [](...){ glfwTerminate(); });

    try {
        Engine engine;
        Editor editor;

        std::string error;
        if(!engine.Init(false, 0.8f, error)) {
            std::cerr << "Error init engine: " << error << std::endl;
            return EXIT_FAILURE;
        }

        if(!editor.Init(engine, error)) {
            std::cerr << "Error init editor: " << error << std::endl;
            return EXIT_FAILURE;
        }

        engine.Run([&editor](Engine& engine){
            editor.Render(engine);
        });

        editor.Destroy();
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
