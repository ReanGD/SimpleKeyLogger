#include "engine/api/gl.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>

bool GLApi::isDXTSupported = false;

bool GLApi::Init(std::string& error) {
    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        error = fmt::format("Failed to initialize GLEW: {}", glewGetErrorString(err));
        return false;
    }

    spdlog::debug("[GPU] Vendor:   {}", glGetString(GL_VENDOR));
    spdlog::debug("[GPU] Renderer: {}", glGetString(GL_RENDERER));
    spdlog::debug("[GPU] Version:  {}", glGetString(GL_VERSION));
    spdlog::debug("[GPU] GLSL:     {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

    GLint numExt = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);

    for (GLuint i=0; i!=static_cast<GLuint>(numExt); ++i) {
        // GLubyte
        const char* name = reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i));
        spdlog::trace("Supported extension: {}", name);

        // DDS texture compression support
        if ((strcmp(name, "GL_EXT_texture_compression_s3tc") == 0) ||
            (strcmp(name, "GL_WEBGL_compressed_texture_s3tc") == 0) ||
            (strcmp(name, "GL_WEBKIT_WEBGL_compressed_texture_s3tc") == 0)) {
                GLApi::isDXTSupported = true;
        }
    }

    if (GLApi::isDXTSupported) {
        spdlog::debug("[EXTENSION] DXT compressed textures supported");
    } else {
        spdlog::debug("[EXTENSION] DXT compressed textures unsupported");
    }

    GLenum params[] = {
        GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
        GL_MAX_CUBE_MAP_TEXTURE_SIZE,
        GL_MAX_DRAW_BUFFERS,
        GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
        GL_MAX_TEXTURE_IMAGE_UNITS,
        GL_MAX_TEXTURE_SIZE,
        GL_MAX_VARYING_FLOATS,
        GL_MAX_VERTEX_ATTRIBS,
        GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
        GL_MAX_VERTEX_UNIFORM_COMPONENTS,
        GL_MAX_VIEWPORT_DIMS,
    };
    const char* paramNames[] = {
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
        "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
        "GL_MAX_DRAW_BUFFERS",
        "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
        "GL_MAX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_TEXTURE_SIZE",
        "GL_MAX_VARYING_FLOATS",
        "GL_MAX_VERTEX_ATTRIBS",
        "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
        "GL_MAX_VIEWPORT_DIMS",
    };

    spdlog::debug("GL Context Params:");
    for (size_t i = 0; i != sizeof(params) / sizeof(params[0]) ; ++i) {
        int v = 0;
        glGetIntegerv(params[i], &v);
        spdlog::debug("{} {}", paramNames[i], v);
    }

    return true;
}
