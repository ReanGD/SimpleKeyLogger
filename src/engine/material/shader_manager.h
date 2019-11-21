#pragma once

#include <string>
#include "engine/common/noncopyable.h"


class ShaderManager : Noncopyable {
public:
    ShaderManager() = default;
    ~ShaderManager() = default;
public:
    static ShaderManager& Get() noexcept {
        static ShaderManager instance;
        return instance;
    }

    void Create();
private:
};
