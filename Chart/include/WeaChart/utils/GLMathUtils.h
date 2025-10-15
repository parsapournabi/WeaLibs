#ifndef GLMATHUTILS_H
#define GLMATHUTILS_H

#include <QObject>

namespace GLutils {

/// @details DataType normalize.
/// example -> normalize(quint8 128 ) => 0.50196
/// example -> normalize(float 0.5 ) => 0.5
/// exmaple -> normalize(quint16 32768) => 0.5
/// exmaple -> normalize(qint8 -64) => 0.25
/// @note -> Supported T: int, uint, float, double
template <typename T>
inline float normalize_to_float(T value) {
    if constexpr (std::is_floating_point_v<T>) { // float/double type.
        return static_cast<float>(value);
    }
    else if constexpr (std::is_integral_v<T>) { // Unsigned int.
        if constexpr (std::is_signed_v<T>) {
            float max_abs = std::numeric_limits<T>::max();
            float min_val = std::numeric_limits<T>::min();
            return static_cast<float>(value - min_val) / static_cast<float>(max_abs - min_val);
        }
        else
            return static_cast<float>(value) / static_cast<float>(std::numeric_limits<T>::max());
    }
    else { // Error
        static_assert(std::is_arithmetic_v<T>, "ERROR At normalize_to_float(T value): Type T must be numeric!");
        return 0.0f;
    }
}

/// @details DataType denormalizer
/// exmaple -> denormalize<quint8>(0.5f) => 128
/// exmaple -> denormalize<qint16>(0.25f) => 8192
/// @note -> Supported T: uint & int.
template <typename T>
inline T denormalize(float value) {
    float clamped = std::clamp(value, 0.0f, 1.0f);
    return static_cast<T>(clamped * (float)std::numeric_limits<T>::max() + 0.5f); // Rounding for 0.5f
}

/// @details Normalizing to quint8 (uses for flexible color).
template<typename T>
inline quint8 normalize_to_uint8(T value) {
    static_assert(std::is_arithmetic_v<T>, "ERROR At normalize_to_uint8(T value): Type T must be numeric!");

    if constexpr (std::is_floating_point_v<T>) {
        T clamped = std::clamp(value, T(0), T(1));
        return static_cast<quint8>(clamped * 255 + T(0.5)); // Rounding for 0.5
    }
    else if constexpr (std::is_integral_v<T>) {
        return static_cast<quint8>(std::clamp(value, T(0), T(255)));
    }
}

/// @details This method is useful when you want to have flexible QVector<YourStruct> Assignment.
/// @brief Copying to specific struct using another Vector<Variant>.
/// @param -> dest => Your QVector<Struct> class memeber.
/// @param -> src => Your QVector<Struct> that you want to copy on your class member.
/// @param -> offset => specifying which field must be change. user "offsetof(MyStruct, my_field)"
/// @param -> startIndex => Start index of Destinition QVector<Struct> ("dest").
/// @param -> endIndex => End index of Destinition QVector<Struct> ("dest").
/// strcut ColorStruct { quint8 r, g, b, a; };
/// strcut Point { float x, y; ColorStruct color; };
/// example -> To change only "Point.colors.a" field from QVector<Point>:
/// set_struct_field<quint8, Point>(dest, QVector<quint8> alpha, offsetof(Point, colors.a), 0, alpha.size());
/// This will copy alpha vector to dest vector, BUT only "Point.colors.a" will set.
/// example -> To change whole of the dest vector with new QVector<Point>:
/// set_struct_field<Point, Point(dest, QVector<Point> newPoints, 0, 0, newPoints.size());
/// example -> To change only Color (RGB) fields from QVector<Point>:
/// set_struct_field<ColorStruct, Point>(dest, QVector<ColorStruct> colors, offsetof(Point, color), 0, colors.size());
template<typename T, typename Struct>
void set_struct_field(QVector<Struct> &dest, const QVector<T> &src, size_t offset, int startIndex, int endIndex) {

    assert(dest.size() >= startIndex && dest.size() >= endIndex); // Error if inidices are not in range of dest.
    assert(src.size() >= endIndex - startIndex); // Error if indices are not in range of src.
    for (int i = startIndex; i < endIndex; ++i) {
        T* destPtr = reinterpret_cast<T*>(reinterpret_cast<char *>(&dest[i]) + offset);
        memcpy(destPtr, &src[i - startIndex], sizeof(T));
    }
}


/// @brief Checking the Enumeration with current enum value.
template<typename T>
bool hasFlag(T flag, T value) {
    return (value & flag) == flag;
}

};

#endif // GLMATHUTILS_H
