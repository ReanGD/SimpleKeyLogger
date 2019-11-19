#pragma once

#include <map>
#include <filesystem>
#include "engine/common/noncopyable.h"


class FileManager : Noncopyable {
private:
    FileManager() = default;
    ~FileManager() = default;

public:
    static FileManager& Get() noexcept {
        static FileManager instance;
        return instance;
    }

    void AddRootAlias(const std::string& alias, const std::filesystem::path& path);

    bool GetRealPath(const std::filesystem::path& inPath, std::filesystem::path& outPath, std::string& error) const noexcept;
    bool GetRealPath(const std::filesystem::path& inPath, std::filesystem::path& outPath) const noexcept;
    std::filesystem::path GetRealPath(const std::filesystem::path& inPath) const;

private:
    std::map<std::string, std::filesystem::path> m_aliases;
};
