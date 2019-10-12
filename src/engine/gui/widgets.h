#pragma once
#include <string>

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
    bool InputScalar(const char* label, DataType dataType, void* value, const void* step, const void* stepFast, const char* format);
    bool Combo(const char* label, size_t& value, const char** items, const size_t count);
}

namespace gui {
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
        bool Combo(const char* label, T& value, const char** items, const T count) {
            size_t valueTmp = static_cast<size_t>(value);
            if (detail::Combo(label, valueTmp, items, static_cast<size_t>(count))) {
                value = static_cast<T>(valueTmp);
                return true;
            }

            return false;
        }
};
