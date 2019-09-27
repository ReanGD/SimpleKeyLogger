#include "engine/api/gl.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>

bool GLApi::IsDXTSupported = false;
bool GLApi::IsIFormatQuerySupported = false;

#define ENUM_ELEMENT(index, value) case value: return #value

std::string GLApi::EnumToString(const GLint value) {
    switch (value) {
    ENUM_ELEMENT(0, GL_NONE);
    ENUM_ELEMENT(1, GL_TRUE);

    ENUM_ELEMENT(0x1400, GL_BYTE);
    ENUM_ELEMENT(0x1401, GL_UNSIGNED_BYTE);
    ENUM_ELEMENT(0x1402, GL_SHORT);
    ENUM_ELEMENT(0x1403, GL_UNSIGNED_SHORT);
    ENUM_ELEMENT(0x1404, GL_INT);
    ENUM_ELEMENT(0x1405, GL_UNSIGNED_INT);
    ENUM_ELEMENT(0x1406, GL_FLOAT);

    ENUM_ELEMENT(0x1903,GL_RED);
    ENUM_ELEMENT(0x1904,GL_GREEN);
    ENUM_ELEMENT(0x1905,GL_BLUE);
    ENUM_ELEMENT(0x1906,GL_ALPHA);
    ENUM_ELEMENT(0x1907,GL_RGB);
    ENUM_ELEMENT(0x1908,GL_RGBA);
    ENUM_ELEMENT(0x1909,GL_LUMINANCE);
    ENUM_ELEMENT(0x190A,GL_LUMINANCE_ALPHA);

    ENUM_ELEMENT(0x8051, GL_RGB8);
    ENUM_ELEMENT(0x8052, GL_RGB10);
    ENUM_ELEMENT(0x8053, GL_RGB12);
    ENUM_ELEMENT(0x8054, GL_RGB16);
    ENUM_ELEMENT(0x8055, GL_RGBA2);
    ENUM_ELEMENT(0x8056, GL_RGBA4);
    ENUM_ELEMENT(0x8057, GL_RGB5_A1);
    ENUM_ELEMENT(0x8058, GL_RGBA8);
    ENUM_ELEMENT(0x8059, GL_RGB10_A2);
    ENUM_ELEMENT(0x805A, GL_RGBA12);
    ENUM_ELEMENT(0x805B, GL_RGBA16);

    ENUM_ELEMENT(0x80E0, GL_BGR);
    ENUM_ELEMENT(0x80E1, GL_BGRA);

    ENUM_ELEMENT(0x8225, GL_COMPRESSED_RED);
    ENUM_ELEMENT(0x8226, GL_COMPRESSED_RG);
    ENUM_ELEMENT(0x8227, GL_RG);
    ENUM_ELEMENT(0x8228, GL_RG_INTEGER);
    ENUM_ELEMENT(0x8229, GL_R8);
    ENUM_ELEMENT(0x822A, GL_R16);
    ENUM_ELEMENT(0x822B, GL_RG8);
    ENUM_ELEMENT(0x822C, GL_RG16);
    ENUM_ELEMENT(0x822D, GL_R16F);
    ENUM_ELEMENT(0x822E, GL_R32F);
    ENUM_ELEMENT(0x822F, GL_RG16F);
    ENUM_ELEMENT(0x8230, GL_RG32F);
    ENUM_ELEMENT(0x8231, GL_R8I);
    ENUM_ELEMENT(0x8232, GL_R8UI);
    ENUM_ELEMENT(0x8233, GL_R16I);
    ENUM_ELEMENT(0x8234, GL_R16UI);
    ENUM_ELEMENT(0x8235, GL_R32I);
    ENUM_ELEMENT(0x8236, GL_R32UI);
    ENUM_ELEMENT(0x8237, GL_RG8I);
    ENUM_ELEMENT(0x8238, GL_RG8UI);
    ENUM_ELEMENT(0x8239, GL_RG16I);
    ENUM_ELEMENT(0x823A, GL_RG16UI);
    ENUM_ELEMENT(0x823B, GL_RG32I);
    ENUM_ELEMENT(0x823C, GL_RG32UI);

    ENUM_ELEMENT(0x8362, GL_UNSIGNED_BYTE_2_3_3_REV);
    ENUM_ELEMENT(0x8363, GL_UNSIGNED_SHORT_5_6_5);
    ENUM_ELEMENT(0x8364, GL_UNSIGNED_SHORT_5_6_5_REV);
    ENUM_ELEMENT(0x8365, GL_UNSIGNED_SHORT_4_4_4_4_REV);
    ENUM_ELEMENT(0x8366, GL_UNSIGNED_SHORT_1_5_5_5_REV);
    ENUM_ELEMENT(0x8367, GL_UNSIGNED_INT_8_8_8_8_REV);

    ENUM_ELEMENT(0x83F0, GL_COMPRESSED_RGB_S3TC_DXT1_ANGLE);
    ENUM_ELEMENT(0x83F1, GL_COMPRESSED_RGBA_S3TC_DXT1_ANGLE);
    ENUM_ELEMENT(0x83F2, GL_COMPRESSED_RGBA_S3TC_DXT3_ANGLE);
    ENUM_ELEMENT(0x83F3, GL_COMPRESSED_RGBA_S3TC_DXT5_ANGLE);

    ENUM_ELEMENT(0x8814, GL_RGBA32F);
    ENUM_ELEMENT(0x8815, GL_RGB32F);
    ENUM_ELEMENT(0x881A, GL_RGBA16F);
    ENUM_ELEMENT(0x881B, GL_RGB16F);

    ENUM_ELEMENT(0x8D62, GL_RGB565);

    default: return fmt::format("unknown value '{:x}'", value);
    }

}

static void LogContextParams() {
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

    spdlog::trace("");
    spdlog::trace("GL Context Params:");
    for (size_t i = 0; i != sizeof(params) / sizeof(params[0]) ; ++i) {
        int v = 0;
        glGetIntegerv(params[i], &v);
        spdlog::trace("{} {}", paramNames[i], v);
    }
}

static void LogTextureFormatInfo(GLenum internalformat) {
    GLint preferred, supported, imageFormat, imageType;

    glGetInternalformativ(GL_TEXTURE_2D, internalformat, GL_INTERNALFORMAT_PREFERRED, 1, &preferred);
    glGetInternalformativ(GL_TEXTURE_2D, internalformat, GL_INTERNALFORMAT_SUPPORTED, 1, &supported);
    glGetInternalformativ(GL_TEXTURE_2D, internalformat, GL_TEXTURE_IMAGE_FORMAT, 1, &imageFormat);
    glGetInternalformativ(GL_TEXTURE_2D, internalformat, GL_TEXTURE_IMAGE_TYPE, 1, &imageType);

    spdlog::trace("{}->{} {} = {} : {}",
        GLApi::EnumToString(static_cast<GLint>(internalformat)),
        GLApi::EnumToString(preferred),
        GLApi::EnumToString(supported),
        GLApi::EnumToString(imageFormat),
        GLApi::EnumToString(imageType)
    );
}

static void LogTextureFormatsInfo() {
    if (!GLApi::IsIFormatQuerySupported) {
        return;
    }

    spdlog::trace("");
    spdlog::trace("Texture internal format 'ORIGIN -> PREFERRED SUPPORTED = IMAGE_FORMAT : IMAGE_TYPE':");

    for(const auto& format: {GL_R8, GL_R16, GL_R32F,
                             GL_RG8, GL_RG16, GL_RG32F,
                             GL_RGB565, GL_RGB8, GL_RGB16, GL_RGB32F,
                             GL_RGBA4, GL_RGB5_A1, GL_RGBA8, GL_RGBA16, GL_RGBA32F,
                             GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
                             GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT}) {
        LogTextureFormatInfo(static_cast<GLenum>(format));
    }
}



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

        // DDS texture compression supported
        if ((strcmp(name, "GL_EXT_texture_compression_s3tc") == 0) ||
            (strcmp(name, "GL_WEBGL_compressed_texture_s3tc") == 0) ||
            (strcmp(name, "GL_WEBKIT_WEBGL_compressed_texture_s3tc") == 0)) {
                GLApi::IsDXTSupported = true;
        } else
        // glGetInternalformativ supported
        if ((strcmp(name, "GL_ARB_internalformat_query") == 0)) {
                GLApi::IsIFormatQuerySupported = true;
        }
    }

    spdlog::debug("[EXTENSION][{}] DXT compressed textures supported", GLApi::IsDXTSupported ? "YES" : "NO");
    spdlog::debug("[EXTENSION][{}] glGetInternalformativ supported", GLApi::IsIFormatQuerySupported ? "YES" : "NO");

    LogContextParams();
    LogTextureFormatsInfo();

    return true;
}
