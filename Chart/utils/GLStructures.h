#ifndef GLSTRUCTURES_H
#define GLSTRUCTURES_H

#include "GLMathUtils.h"
#include "GLEnums.h"

#include <QVector2D>
#include <QVector4D>
#include <QDebug>
#include <QColor>

struct Projection {
    qreal left;
    qreal right;
    qreal bottom;
    qreal top;
};

struct SeriesProps {
    int startIndex, endIndex;
    GLMarkerShape markerShape; // Shape: 0 == Square, 1 == Circle, 2 == Texture
    int markerSize;
    GLColorType colorType;
    GLSeriesType type;
    double padding; // Required because on chart.vert std430 has padding for 16Bytes
    QVector4D color;
};
static_assert(sizeof(SeriesProps) % 16 == 0, "Struct SeriesProps must be multiple of 16 bytes");

union ChartColor {
    struct {
        quint8 r, g, b, a;
    };
    quint32 rgba;
    quint8 channels[4];
};

struct PointXYBase {
    // Fields
    QVector2D position;
    ChartColor color;

    // Constructures
    explicit PointXYBase()
        :
        position(0, 0),
        color{0, 0, 0, 0} {}

    PointXYBase(const QPointF &_position, ChartColor _color = {0, 0, 0, 0})
        :
        position(QVector2D(_position.x(), _position.y())),
        color{_color} {}

    PointXYBase(QPoint _position, ChartColor _color = {0, 0, 0, 0})
        :
        position(QVector2D(_position.x(), _position.y())),
        color{_color} {}

    PointXYBase(QVector2D _position, ChartColor _color = {0, 0, 0, 0})
        :
        position(_position),
        color{_color} {}

    PointXYBase(float x, float y, ChartColor _color = {0, 0, 0, 0})
        :
        position(x, y),
        color{_color} {}

    template<typename T1, typename T2, typename T3, typename T4>
    PointXYBase(QVector2D _position, T1 red = 0, T2 green = 0, T3 blue = 0, T4 alpha = 0)
        :
        position(_position),
        color {
            GLutils::normalize_to_uint8(red),
            GLutils::normalize_to_uint8(green),
            GLutils::normalize_to_uint8(blue),
            GLutils::normalize_to_uint8(alpha)
        } {}

    template<typename T1, typename T2, typename T3, typename T4>
    PointXYBase(float x, float y, T1 red = 0, T2 green = 0, T3 blue = 0, T4 alpha = 0)
        :
        position(x, y),
        color {
            GLutils::normalize_to_uint8(red),
            GLutils::normalize_to_uint8(green),
            GLutils::normalize_to_uint8(blue),
            GLutils::normalize_to_uint8(alpha)
        } {}

    // Virtual base methods & Properties
    QString name() const { return "PointXYBase"; }

    inline float x() const { return position.x(); }
    inline void setX(float x) { position.setX(x); }

    inline float y() const { return position.y(); }
    inline void setY(float y) { position.setY(y); }

    inline quint8 red() const { return color.r; }
    template <typename T> inline void setRed(T r) { color.r = GLutils::normalize_to_uint8(r); }

    inline quint8 green() const { return color.g; }
    template <typename T> inline void setGreen(T g) { color.g = GLutils::normalize_to_uint8(g); }

    inline quint8 blue() const { return color.b; }
    template <typename T> inline void setBlue(T b) { color.b = GLutils::normalize_to_uint8(b); }

    inline quint8 alpha() const { return color.a; }
    template <typename T> inline void setAlpha(T a) { color.a = GLutils::normalize_to_uint8(a); }

    inline quint32 rgba() const { return color.rgba; }
    inline void setRgba(quint32 rgba) { color.rgba = rgba; }

};
Q_DECLARE_METATYPE(PointXYBase)
Q_DECLARE_METATYPE(QVector<PointXYBase>)


#endif // GLSTRUCTURES_H
