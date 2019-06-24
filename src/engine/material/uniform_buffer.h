#pragma once

#include <string>
#include <glm/mat4x4.hpp>


class UniformBuffer {
public:
    UniformBuffer() = delete;
    UniformBuffer(const UniformBuffer&) = delete;
    UniformBuffer(UniformBuffer&&) = delete;
    UniformBuffer& operator=(const UniformBuffer&) = delete;
    UniformBuffer& operator=(UniformBuffer&&) = delete;

    UniformBuffer(size_t size);
    ~UniformBuffer();

    template <typename T>
    struct is_supported_type {
        using type = typename std::enable_if<
                std::is_same<int, T>::value ||
                std::is_same<float, T>::value ||
                std::is_same<glm::vec2, T>::value ||
                std::is_same<glm::vec3, T>::value ||
                std::is_same<glm::vec4, T>::value ||
                std::is_same<glm::mat2, T>::value ||
                std::is_same<glm::mat3, T>::value ||
                std::is_same<glm::mat4, T>::value
        >::type;
    };

    template <typename T, typename = typename is_supported_type<T>::type>
    void setUniform(size_t offset, const T& v) noexcept {
        if ((sizeof(v) + offset) <= m_size) {
            T* dst = reinterpret_cast<T*>(m_buffer + offset);
            *dst = v;
        }
    }

    void Sync();
    void Bind(uint index) const;
private:
    uint m_handle = 0;
    size_t m_size = 0;
    uint8_t* m_buffer = nullptr;
};
