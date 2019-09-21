#pragma once

#include <string>
#include "engine/material/image.h"


bool LoadImage(const char *filename, Image& image, std::string& error);
void FreeImage(void* data);