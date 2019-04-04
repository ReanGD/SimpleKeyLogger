#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <nonstd/expected.hpp>

nonstd::expected<GLuint, std::string> LoadShader(const char* filepath, GLenum shaderType);
