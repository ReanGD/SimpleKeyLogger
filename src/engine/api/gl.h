#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>


struct GLApi {
    static bool Init(std::string& error);
};
