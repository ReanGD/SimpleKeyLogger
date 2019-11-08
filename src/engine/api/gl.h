#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>


struct GLApi {
    static bool IsDXTSupported; // DDS texture compression supported
    static bool IsIFormatQuerySupported; // glGetInternalformativ supported

    static std::string EnumToString(const GLint value);

    static void Init();
};
