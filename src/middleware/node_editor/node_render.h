#pragma once

#include <map>
#include "engine/gui/math.h"


class GradientColor {
public:
    GradientColor() = default;

    void Add(double gradientPos, const math::Color& gradientColor);
    size_t Count() const noexcept { return m_points.size(); }
    const math::Color Get(double gradientPos) const;
    void Clear();

private:
    // pos => color
    std::map<double, math::Color> m_points;
};
