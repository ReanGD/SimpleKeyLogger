#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>


struct GLApi {
    static bool isDXTSupported; // DDS texture compression support

    static bool Init(std::string& error);
};
