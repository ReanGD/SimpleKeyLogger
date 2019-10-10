#include "engine/gui/widgets.h"

#include <imgui.h>
#include <exception>
#include <fmt/format.h>


namespace detail {
static ImGuiDataType_ ToImGui(DataType value) {
    switch (value) {
        case DataType::S8: return ImGuiDataType_S8;
        case DataType::U8: return ImGuiDataType_U8;
        case DataType::S16: return ImGuiDataType_S16;
        case DataType::U16: return ImGuiDataType_U16;
        case DataType::S32: return ImGuiDataType_S32;
        case DataType::U32: return ImGuiDataType_U32;
        case DataType::S64: return ImGuiDataType_S64;
        case DataType::U64: return ImGuiDataType_U64;
        case DataType::Float: return ImGuiDataType_Float;
        case DataType::Double: return ImGuiDataType_Double;
        default:
            throw std::runtime_error(fmt::format("unknown value of widget type: '{}'", static_cast<uint8_t>(value)).c_str());
    }
}

bool DragScalar(const char* label, DataType dataType, void* value, float speed, const void* minValue, const void* maxValue, const char* format, float power) {
    return ImGui::DragScalar(label, ToImGui(dataType), value, speed, minValue, maxValue, format, power);
}

bool InputScalar(const char* label, DataType dataType, void* value, const void* step, const void* stepFast, const char* format) {
    // TODO: ImGuiInputTextFlags
    return ImGui::InputScalar(label, ToImGui(dataType), value, step, stepFast, format, ImGuiInputTextFlags(0));
}
}