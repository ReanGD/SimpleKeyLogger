#pragma once

#include <string>


class Texture {
public:
    Texture() = default;
    ~Texture() = default;

    bool Load(const std::string& path, std::string& error);

    void Bind(uint unit) const;
    void Unbind(uint unit) const;
    void Delete();
private:
    uint m_handle = 0;
};
