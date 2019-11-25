#include "engine/common/to_string.h"

#include <fmt/format.h>


std::string ToString(const glm::mat4& x) {
    std::string formatStr("mat4x4(\n  ({: .2f}, {: .2f}, {: .2f}, {: .2f}),\n  ({: .2f}, {: .2f}, {: .2f}, {: .2f}),\n  ({: .2f}, {: .2f}, {: .2f}, {: .2f}),\n  ({: .2f}, {: .2f}, {: .2f}, {: .2f})\n)");

    return fmt::format(formatStr,
        x[0][0], x[0][1], x[0][2], x[0][3],
        x[1][0], x[1][1], x[1][2], x[1][3],
        x[2][0], x[2][1], x[2][2], x[2][3],
        x[3][0], x[3][1], x[3][2], x[3][3]);
}

std::string ToString(const glm::mat3& x) {
    std::string formatStr("mat3x3(\n  ({: .2f}, {: .2f}, {: .2f},\n  ({: .2f}, {: .2f}, {: .2f},\n  ({: .2f}, {: .2f}, {: .2f},\n)");

    return fmt::format(formatStr,
        x[0][0], x[0][1], x[0][2],
        x[1][0], x[1][1], x[1][2],
        x[2][0], x[2][1], x[2][2]);
}
