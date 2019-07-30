#pragma once

#include <string>


class Texture {
public:
    Texture() = default;
    ~Texture() = default;

    bool Create(uint32_t width, uint32_t height, std::string& error);
    bool Load(const std::string& path, std::string& error);

    void Bind(uint unit) const;
    void Unbind(uint unit) const;
    void Destroy();
private:
    uint m_handle = 0;
};
