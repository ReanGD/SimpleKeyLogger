#pragma once
#include <memory>
#include <string>
#include "engine/common/math.h"


class Texture;

namespace gui {
namespace detail {

    enum class DataType : uint8_t {
        S8,       // signed char / char (with sensible compilers)
        U8,       // unsigned char
        S16,      // short
        U16,      // unsigned short
        S32,      // int
        U32,      // unsigned int
        S64,      // long long / __int64
        U64,      // unsigned long long / unsigned __int64
        Float,    // float
        Double,   // double
        Unknown
    };

    template<typename T> constexpr DataType TypeID() { return DataType::Unknown; }
    template<> constexpr DataType TypeID<char>() { return DataType::S8; }
    template<> constexpr DataType TypeID<unsigned char>() { return DataType::U8; }
    template<> constexpr DataType TypeID<short>() { return DataType::S16; }
    template<> constexpr DataType TypeID<unsigned short>() { return DataType::U16; }
    template<> constexpr DataType TypeID<int>() { return DataType::S32; }
    template<> constexpr DataType TypeID<unsigned int>() { return DataType::U32; }
    template<> constexpr DataType TypeID<long long>() { return DataType::S64; }
    template<> constexpr DataType TypeID<unsigned long long>() { return DataType::U64; }
    template<> constexpr DataType TypeID<float>() { return DataType::Float; }
    template<> constexpr DataType TypeID<double>() { return DataType::Double; }

    template <typename T>
    struct isSupportedType {
        using type = typename std::enable_if<
        std::is_same<char, T>::value ||
        std::is_same<unsigned char, T>::value ||
        std::is_same<short, T>::value ||
        std::is_same<unsigned short, T>::value ||
        std::is_same<int, T>::value ||
        std::is_same<unsigned int, T>::value ||
        std::is_same<long long, T>::value ||
        std::is_same<unsigned long, T>::value ||
        std::is_same<float, T>::value ||
        std::is_same<double, T>::value
        >::type;
    };

    template <typename T>
    struct isIntegerType {
        using type = typename std::enable_if<
        std::is_same<char, T>::value ||
        std::is_same<unsigned char, T>::value ||
        std::is_same<short, T>::value ||
        std::is_same<unsigned short, T>::value ||
        std::is_same<int, T>::value ||
        std::is_same<unsigned int, T>::value ||
        std::is_same<long long, T>::value ||
        std::is_same<unsigned long, T>::value
        >::type;
    };

    template <typename T>
    struct isFloatType {
        using type = typename std::enable_if<
        std::is_same<float, T>::value ||
        std::is_same<double, T>::value
        >::type;
    };

    bool DragScalar(const char* label, DataType dataType, void* value, float speed, const void* minValue, const void* maxValue, const char* format, float power = 1.0f);
    bool SliderScalar(const char* label, DataType dataType, void* value, const void* minValue, const void* maxValue, const char* format, float power = 1.0f);
    bool InputScalar(const char* label, DataType dataType, void* value, const void* step, const void* stepFast, const char* format);
    bool Combo(const char* label, size_t& value, const char** items, const size_t count);

} // end namespace gui::detail

enum class IconType : uint8_t {
    Flow,
    Circle,
    Square,
    Grid,
    RoundSquare,
    Diamond
};

template<typename T> struct Step {
    Step(const T& normal, const T& fast) : normal(normal), fast(fast) {}

    const T& normal;
    const T& fast;
};

template<typename T> struct Range {
    Range(const T& minValue, const T& maxValue) : minValue(minValue), maxValue(maxValue) {}
    bool Check(const T& value) const noexcept { return ((value >= minValue) && (value <= maxValue)); }

    const T& minValue;
    const T& maxValue;
};


template<typename T, typename = typename detail::isSupportedType<T>::type>
    bool DragScalar(const char* label, T& value, float speed, const T& minValue, const T& maxValue, const char* format = nullptr, float power = 1.0f) {
        return detail::DragScalar(label, detail::TypeID<T>(), &value, speed, &minValue, &maxValue, format, power);
    }

template<typename T, typename = typename detail::isIntegerType<T>::type>
    bool DragScalarI(const char* label, T& value, float speed, const T& minValue, const T& maxValue, const char* format = nullptr) {
        return detail::DragScalar(label, detail::TypeID<T>(), &value, speed, &minValue, &maxValue, format);
    }

template<typename T, typename = typename detail::isFloatType<T>::type>
    bool DragScalarF(const char* label, T& value, float speed, const T& minValue, const T& maxValue, const char* format = nullptr, float power = 1.0f) {
        return detail::DragScalar(label, detail::TypeID<T>(), &value, speed, &minValue, &maxValue, format, power);
    }

template<typename T, typename = typename detail::isSupportedType<T>::type>
    bool InputScalar(const char* label, T& value, const Step<T>& step, const char* format = nullptr) {
        return detail::InputScalar(label, detail::TypeID<T>(), &value, &step.normal, &step.fast, format);
    }

template<typename T, typename = typename detail::isSupportedType<T>::type>
    bool InputScalar(const char* label, T& value, const Step<T>& step, const Range<T>& range, const char* format = nullptr) {
        T tmpValue = value;
        if (detail::InputScalar(label, detail::TypeID<T>(), &tmpValue, &step.normal, &step.fast, format)) {
            if (range.Check(tmpValue)) {
                value = tmpValue;
                return true;
            }
        }

        return false;
    }


template<typename T>
    bool SliderEnum(const char* label, T& value, const char** items, const T count) {
        uint64_t minValue = 0;
        uint64_t maxValue = static_cast<uint64_t>(count - 1);
        uint64_t valueTmp = static_cast<uint64_t>(value);
        if (detail::SliderScalar(label, detail::DataType::U64, &valueTmp, &minValue, &maxValue, items[valueTmp])) {
            value = static_cast<T>(valueTmp);
            return true;
        }

        return false;
    }

template<typename T>
    bool Combo(const char* label, T& value, const char** items, const T count) {
        size_t valueTmp = static_cast<size_t>(value);
        if (detail::Combo(label, valueTmp, items, static_cast<size_t>(count))) {
            value = static_cast<T>(valueTmp);
            return true;
        }

        return false;
    }

bool Checkbox(const char* label, bool& value);

void NodeIcon(const math::Size& size, gui::IconType type, bool filled, math::Color color = math::Color(1, 1, 1, 1), math::Color innerColor = math::Color(0, 0, 0, 0));
void Image(const std::shared_ptr<Texture>& texture, const math::Size& size,
    const math::Pointf& uv0 = math::Pointf(0, 1), const math::Pointf& uv1 = math::Pointf(1, 0),
    math::Color tintCol = math::Color(255), math::Color borderCol = math::Color(0));

} // end namespace gui
