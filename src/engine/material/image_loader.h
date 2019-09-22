#pragma once

#include "engine/material/image.h"


namespace image_loader {
    bool Load(const char *filename, Image& image, std::string& error);
    void Free(Image& image);
};
