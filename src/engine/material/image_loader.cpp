#include "engine/material/image_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fmt/format.h>


bool image_loader::Load(const char *filename, Image& image, std::string& error) {
    stbi_set_flip_vertically_on_load(true);
    FILE *f = stbi__fopen(filename, "rb");
    if (f == nullptr) {
        error = fmt::format("failed to open a image file '{}'", filename);
        return false;
    }

    stbi__context s;
    stbi__start_file(&s, f);
    long pos = ftell(f);
    bool is16bit = (stbi__is_16_main(&s) == 1);
    fseek(f,pos,SEEK_SET);

    int width = 0;
    int height = 0;
    int channels = 0;
    void* data = nullptr;
    if (is16bit) {
        data = stbi__load_and_postprocess_16bit(&s, &width, &height, &channels, STBI_default);
    } else {
        data = stbi__load_and_postprocess_8bit(&s, &width, &height, &channels, STBI_default);
    }
    // stbi_loadf_from_file
    fclose(f);

    if (data == nullptr) {
        error = fmt::format("failed to load a image from file '{}', error: '{}'", filename, stbi_failure_reason());
        return false;
    }

    PixelFormat format;
    switch(channels) {
        case 1: format = is16bit ? PixelFormat::R16 : PixelFormat::R8; break;
        case 2: format = is16bit ? PixelFormat::RG16 : PixelFormat::RG8; break;
        case 3: format = is16bit ? PixelFormat::RGB16 : PixelFormat::RGB8; break;
        case 4: format = is16bit ? PixelFormat::RGBA16 : PixelFormat::RGBA8; break;
        default:
            error = fmt::format("failed to parse a image file '{}', error: wrong value of 'channels' = {}", filename, channels);
            stbi_image_free(data);
            return false;
    }

    ImageHeader header(static_cast<uint32_t>(width), static_cast<uint32_t>(height), format);
    image = Image(header, 1, data);

    return true;
}

void image_loader::Free(Image& image) {
    if (image.data != nullptr) {
        stbi_image_free(image.data);
    }
}
