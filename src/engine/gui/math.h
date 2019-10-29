#pragma once

#include <cstdint>


namespace math {

struct Color {
    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : red(r), green(g), blue(b), alpha(a) {}
    Color(uint8_t v) : Color(v, v, v, v) {}

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

template <typename T>
struct BasicPoint {
    BasicPoint() = default;
    BasicPoint(T x, T y): x(x), y(y) {}

    T x = 0;
    T y = 0;
};

using Point = BasicPoint<int32_t>;
using Pointf = BasicPoint<float>;

template <typename T>
struct BasicSize {
    BasicSize(): w(0), h(0) {}
    BasicSize(T w, T h): w(w), h(h) {}

    T w = 0;
    T h = 0;
};

using Size = BasicSize<uint32_t>;
using Sizef = BasicSize<float>;

template <typename T>
struct BasicRect {
    using point_t = BasicPoint<T>;

    BasicRect() = default;
    BasicRect(const point_t& pos1, const point_t& pos2): x(pos1.x), y(pos1.y), w(pos2.x - pos1.x), h(pos2.y - pos1.y) {}
    BasicRect(T x, T y, T w, T h): x(x), y(y), w(w), h(h) {}

    T Left() const { return x; }
    T Right() const { return x + w; }
    T Top() const { return y; }
    T Bottom() const { return y + h; }

    T CenterX() const { return x + w / 2; }
    T CenterY() const { return y + h / 2; }
    point_t Center() const { return point_t(CenterX(), CenterY()); }

    T x = 0;
    T y = 0;
    T w = 0;
    T h = 0;
};

using Rect = BasicRect<int32_t>;
using Rectf = BasicRect<float>;

}
