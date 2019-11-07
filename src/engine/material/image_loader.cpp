#include "engine/material/image.h"

#define STBI_NO_PSD
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "engine/common/exception.h"


void Image::Load(const char *filename, bool verticallyFlip) {
    Destroy();

    stbi_set_flip_vertically_on_load(true);
    FILE *f = stbi__fopen(filename, "rb");
    if (f == nullptr) {
        throw EngineError("failed to open a image file '{}'", filename);
    }

    stbi__context s;
    stbi__start_file(&s, f);

    int width = 0;
    int height = 0;
    int channels = 0;
    stbi__result_info ri;
    stbi__vertically_flip_on_load = verticallyFlip;
    void* data = stbi__load_main(&s, &width, &height, &channels, STBI_default, &ri, 0);
    if (stbi__vertically_flip_on_load) {
        stbi__vertical_flip(data, width, height, channels * ri.bits_per_channel / 8);
    }

    fclose(f);

    if (data == nullptr) {
        throw EngineError("failed to load a image from file '{}', error: '{}'", filename, stbi_failure_reason());
    }

    if ((ri.bits_per_channel != 16) && (ri.bits_per_channel != 8)) {
        throw EngineError("failed to load a image from file '{}', error: unknown bits per channel value = {}", filename, ri.bits_per_channel);
    }

    bool is16bit = (ri.bits_per_channel == 16);

    PixelFormat format;
    switch(channels) {
        case 1: format = is16bit ? PixelFormat::R16 : PixelFormat::R8; break;
        case 2: format = is16bit ? PixelFormat::RG16 : PixelFormat::RG8; break;
        case 3: format = is16bit ? PixelFormat::RGB16 : PixelFormat::RGB8; break;
        case 4: format = is16bit ? PixelFormat::RGBA16 : PixelFormat::RGBA8; break;
        default:
            stbi_image_free(data);
            throw EngineError("failed to parse a image file '{}', error: wrong value of 'channels' = {}", filename, channels);
    }

    ImageHeader header(static_cast<uint32_t>(width), static_cast<uint32_t>(height), format);
    view = ImageView(header, 1, data);
    deleter = [](void* data) {
        if (data != nullptr) {
            stbi_image_free(data);
        }
    };
}
