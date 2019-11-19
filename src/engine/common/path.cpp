#include "engine/common/path.h"

#include <numeric>
#include "engine/common/exception.h"


void FileManager::AddRootAlias(const std::string& alias, const std::filesystem::path& path) {
    m_aliases[alias] = path;
}

bool FileManager::GetRealPath(const std::filesystem::path& inPath, std::filesystem::path& outPath, std::string& error) const noexcept {
    if (inPath.empty()) {
        error = fmt::format("not found real path for empty path");
        return false;
    }

    if (std::filesystem::exists(inPath)) {
        outPath = inPath;
        return true;
    }

    const auto firstPathElement = inPath.begin();
    const auto it = m_aliases.find(*firstPathElement);
    if (it != m_aliases.cend()) {
        outPath = std::accumulate(std::next(firstPathElement), inPath.end(), it->second, std::divides{});
        if (std::filesystem::exists(outPath)) {
            return true;
        }
    }

    error = fmt::format("not found real path for: {}", inPath.c_str());
    return false;
}

bool FileManager::GetRealPath(const std::filesystem::path& inPath, std::filesystem::path& outPath) const noexcept {
    std::string error;
    return GetRealPath(inPath, outPath, error);
}

std::filesystem::path FileManager::GetRealPath(const std::filesystem::path& inPath) const {
    std::string error;
    std::filesystem::path result;
    if (!GetRealPath(inPath, result, error)) {
        throw EngineError(error);
    }

    return result;
}
