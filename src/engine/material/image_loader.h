#pragma once

#include <string>


void* LoadImage(const char *filename, int& width, int& height, int& channels, bool& is16bit, std::string& error);
void FreeImage(void* data);