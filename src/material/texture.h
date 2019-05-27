#pragma once

#include <string>


class Texture {
private:
    Texture() = default;
    Texture(uint handle);
public:
    ~Texture() = default;

    void Bind() const;
    void Unbind() const;
    void Delete();

    static std::pair<Texture, const std::string> Create(const std::string& path);
private:
    uint m_handle = 0;
};
