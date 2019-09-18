#include "engine/material/image_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fmt/format.h>


void* LoadImage(const char *filename, int& width, int& height, int& channels, bool& is16bit, std::string& error) {
    void *data = nullptr;

    stbi_set_flip_vertically_on_load(true);
    FILE *f = stbi__fopen(filename, "rb");
    if (f == nullptr) {
        error = fmt::format("failed to open a image file '{}'", filename);
        return data;
    }

    stbi__context s;
    stbi__start_file(&s, f);
    long pos = ftell(f);
    is16bit = (stbi__is_16_main(&s) == 1);
    fseek(f,pos,SEEK_SET);

    if (is16bit) {
        data = stbi__load_and_postprocess_16bit(&s, &width, &height, &channels, STBI_default);
    } else {
        data = stbi__load_and_postprocess_8bit(&s, &width, &height, &channels, STBI_default);
    }
    fclose(f);

    if (data == nullptr) {
        error = fmt::format("Failed to load a image from file '{}', error: '{}'", filename, stbi_failure_reason());
        return data;
    }

    return data;
}

void FreeImage(void* data) {
    if (data != nullptr) {
        stbi_image_free(data);
    }
}
