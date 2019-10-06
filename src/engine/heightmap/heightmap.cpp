#include "engine/heightmap/heightmap.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "engine/material/image_loader.h"
#include "engine/material/texture_manager.h"


std::shared_ptr<PhysicalNode> Heightmap::Load(const std::filesystem::path& path, std::string& error) noexcept {
    std::filesystem::path fullPath;
    if (!TextureManager::Get().GetFullPath(path, fullPath, error)) {
        return nullptr;
    }

    Image image;
    if(!image_loader::Load(fullPath.c_str(), image, true, error)) {
        return nullptr;
    }

    if (image.header.format != PixelFormat::R8G8B8) {
        error = fmt::format("wrong image ({}) format for heightmap, expected: {}, actual: '{}'",
            fullPath.c_str(), ToStr(PixelFormat::R8G8B8), ToStr(image.header.format));
        image_loader::Free(image);
        return nullptr;
    }

    if (image.header.width != image.header.height) {
        error = fmt::format("wrong image ({}) size for heightmap, expected: width == height, actual: width = {}, height = {}",
            fullPath.c_str(), image.header.width, image.header.height);
        image_loader::Free(image);
        return nullptr;
    }

    uint8_t* data = reinterpret_cast<uint8_t*>(image.data);
    uint32_t gridSize = image.header.width;
    // float gridSpacing = 0.2f;
    float heightScale = 0.2f;
    size_t bytesPerElement = sizeof(uint8_t);
    float minHeight, maxHeight;

    uint8_t* rawHeightfieldData = new uint8_t[static_cast<size_t>(gridSize) * static_cast<size_t>(gridSize) * bytesPerElement];
    uint8_t* p = rawHeightfieldData;
    for (uint32_t j=0; j!=gridSize; ++j) {
        for (uint32_t i=0; i!=gridSize; ++i) {
            // float x = i * gridSpacing;
            // float y = j * gridSpacing;
            float heightScaling = (14. / 256.);
            float z = static_cast<float>(data[(gridSize - 1 - i) * 3 + gridSize * j * 3]) * heightScaling;
            *p = static_cast<unsigned char>(z / heightScale);

            if (!i && !j) {
                minHeight = z;
                maxHeight = z;
            } else {
                if (z < minHeight) {
                    minHeight = z;
                }
                if (z > maxHeight) {
                    maxHeight = z;
                }
            }

            p += bytesPerElement;
        }
    }

    image_loader::Free(image);

    // TODO: remove rawHeightfieldData
    return std::make_shared<PhysicalTerrain>(gridSize, rawHeightfieldData, heightScale, minHeight, maxHeight);
}
