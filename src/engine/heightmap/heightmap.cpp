#include "engine/heightmap/heightmap.h"

#include "engine/material/texture_manager.h"
#include "engine/common/exception.h"


std::shared_ptr<PhysicalNode> Heightmap::Load(const std::filesystem::path& path) {
    auto fullPath = TextureManager::Get().GetFullPath(path);
    Image image(fullPath.c_str(), true);

    auto header = image.view.header;
    if (header.format != PixelFormat::R8G8B8) {
        throw EngineError("wrong image ({}) format for heightmap, expected: {}, actual: '{}'",
            fullPath.c_str(), ToStr(PixelFormat::R8G8B8), ToStr(header.format));
    }

    if (header.width != header.height) {
        throw EngineError("wrong image ({}) size for heightmap, expected: width == height, actual: width = {}, height = {}",
            fullPath.c_str(), header.width, header.height);
    }

    uint8_t* data = reinterpret_cast<uint8_t*>(image.view.data);
    uint32_t gridSize = header.width;
    float heightScale = 0.1f;
    size_t bytesPerElement = sizeof(uint8_t);
    float minHeight, maxHeight;

    uint8_t* rawHeightfieldData = new uint8_t[static_cast<size_t>(gridSize) * static_cast<size_t>(gridSize) * bytesPerElement];
    uint8_t* p = rawHeightfieldData;
    for (uint32_t j=0; j!=gridSize; ++j) {
        for (uint32_t i=0; i!=gridSize; ++i) {
            uint8_t rawZ = data[i * 3 + gridSize * j * 3];
            float z = static_cast<float>(rawZ) * heightScale;
            *p = rawZ;

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

    // TODO: remove rawHeightfieldData
    return std::make_shared<PhysicalTerrain>(gridSize, rawHeightfieldData, heightScale, minHeight, maxHeight, 0.5f * (minHeight + maxHeight));
}
