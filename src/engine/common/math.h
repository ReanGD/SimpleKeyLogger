#pragma once

#include <cstdint>


namespace math {

struct Color4f {
    Color4f() = default;
    Color4f(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
        : red(static_cast<float>(r) / 255.0f)
        , green(static_cast<float>(g) / 255.0f)
        , blue(static_cast<float>(b) / 255.0f)
        , alpha(static_cast<float>(a) / 255.0f) {}

    Color4f(float r, float g, float b, float a = 1.0f) noexcept : red(r), green(g), blue(b), alpha(a) {}
    Color4f(float v) noexcept : Color4f(v, v, v, v) {}

    union {
        float value[4] = {0, 0, 0, 1.0f};
        struct {
            float red;
            float green;
            float blue;
            float alpha;
        };
    };
};

struct Color {
    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept : red(r), green(g), blue(b), alpha(a) {}
    Color(uint8_t v) noexcept : Color(v, v, v, v) {}

    Color4f ToColor4f() const noexcept {
        return Color4f(red, green, blue, alpha);
    }

    union {
        uint32_t value = 0xFF000000;
        struct {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            uint8_t alpha;
        };
    };
};

using Color4 = Color;

struct Color3f {
    Color3f() = default;
    Color3f(uint8_t r, uint8_t g, uint8_t b) noexcept
        : red(static_cast<float>(r) / 255.0f)
        , green(static_cast<float>(g) / 255.0f)
        , blue(static_cast<float>(b) / 255.0f) {}

    Color3f(float r, float g, float b) noexcept : red(r), green(g), blue(b) {}
    Color3f(float v) noexcept : Color3f(v, v, v) {}

    union {
        float value[3] = {0, 0, 0};
        struct {
            float red;
            float green;
            float blue;
        };
    };
};

struct Color3 {
    Color3() = default;
    Color3(uint8_t r, uint8_t g, uint8_t b) noexcept : red(r), green(g), blue(b) {}
    Color3(uint8_t v) noexcept : Color3(v, v, v) {}

    Color3f ToColor3f() const noexcept {
        return Color3f(red, green, blue);
    }

    union {
        uint32_t value = 0;
        struct {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            uint8_t _;
        };
    };
};

template <typename T>
struct BasicPoint {
    using Type = T;

    BasicPoint() = default;
    BasicPoint(T x, T y) noexcept : x(x), y(y) {}

    T x = 0;
    T y = 0;
};

using Point = BasicPoint<uint32_t>;
using PointI = BasicPoint<int32_t>;
using Pointf = BasicPoint<float>;

template <typename T>
struct BasicSize {
    using Type = T;

    BasicSize() noexcept : w(0), h(0) {}
    BasicSize(T w, T h) noexcept : w(w), h(h) {}

    T w = 0;
    T h = 0;
};

using Size = BasicSize<uint32_t>;
using Sizef = BasicSize<float>;

template <typename T>
struct BasicRect {
    using Type = T;
    using PointT = BasicPoint<T>;
    using SizeT = BasicSize<T>;
    using RectT = BasicRect<T>;

    BasicRect() = default;
    BasicRect(const PointT& pos1, const PointT& pos2) noexcept : x(pos1.x), y(pos1.y), w(pos2.x - pos1.x), h(pos2.y - pos1.y) {}
    BasicRect(T x, T y, T w, T h) noexcept : x(x), y(y), w(w), h(h) {}

    T Top() const noexcept { return y; }
    T Bottom() const noexcept { return y + h; }
    T Left() const noexcept { return x; }
    T Right() const noexcept { return x + w; }

    T Width() const noexcept { return w; }
    T Height() const noexcept { return h; }
    template<typename U> BasicSize<U> SizeCast() const noexcept { return BasicSize<U>(static_cast<U>(w), static_cast<U>(h)); }

    T CenterX() const noexcept { return x + w / 2; }
    T CenterY() const noexcept { return y + h / 2; }
    PointT Center() const noexcept { return PointT(CenterX(), CenterY()); }

    RectT CutOffTop(T height) {
        auto result = RectT(x, y, w, height);
        y += height;
        return result;
    }

    RectT CutOffRight(T width) {
        w -= width;
        return RectT(w, y, width, h);
    }

    T x = 0;
    T y = 0;
    T w = 0;
    T h = 0;
};

using Rect = BasicRect<uint32_t>;
using RectI = BasicRect<int32_t>;
using Rectf = BasicRect<float>;

}
