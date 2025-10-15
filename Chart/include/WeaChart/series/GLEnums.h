#ifndef GLENUMS_H
#define GLENUMS_H

#include <QObject>
#include "WeaChart/WeaChart_export.h"

enum GLSeriesType {
    SeriesTypeScatter = 0, // 0
    SeriesTypeLine,
    SeriesTypeArea,
};

enum GLColorType {
    MixBasePointColor = 0, // 0
    OnlyBaseColor,
    OnlyPointColor
};

enum GLMarkerShape {
    ShapeSquare = 0, // 0
    ShapeCircle,
    ShapeTexture
};

enum GLAutoScalePolicy {
    PolicyNone = 0x00,
    PolicyLeft = 0x0001,
    PolicyRight = 0x0002,
    PolicyHCenter = PolicyLeft | PolicyRight,
    PolicyTop = 0x0010,
    PolicyBottom = 0x0020,
    PolicyVCenter = PolicyTop | PolicyBottom,
    PolicyCenter = PolicyHCenter | PolicyVCenter
};

enum GLItemType {
    Line = 0,
    Gate
};

enum GLStyleLine { // Same as Qt::PenStyle
    NoStyle = 0,
    SolidLine,
    DashLine,
    DotLine,
    DashDotLine,
    DashDotDotLine
};


/// @details Each enums must contains at enum of this class.
/// @brief GLMetaEnums will use on QML enums. on com.wearily.WeaChart url
class WEACHART_API GLMetaEnums : public QObject {
    Q_OBJECT
public:
    enum GLSeriesType {
        SeriesTypeScatter = 0, // 0
        SeriesTypeLine,
        SeriesTypeArea,
    };

    enum GLColorType {
        MixBasePointColor = 0, // 0
        OnlyBaseColor,
        OnlyPointColor
    };

    enum GLMarkerShape {
        ShapeSquare = 0, // 0
        ShapeCircle,
        ShapeTexture
    };
    enum GLAutoScalePolicy {
        PolicyNone = 0x00,
        PolicyLeft = 0x0001,
        PolicyRight = 0x0002,
        PolicyHCenter = PolicyLeft | PolicyRight,
        PolicyTop = 0x0010,
        PolicyBottom = 0x0020,
        PolicyVCenter = PolicyTop | PolicyBottom,
        PolicyCenter = PolicyHCenter | PolicyVCenter
    };
    enum GLItemType {
        Line = 0,
        Gate
    };
    enum GLStyleLine { // Same as Qt::PenStyle
        NoStyle = 0,
        SolidLine,
        DashLine,
        DotLine,
        DashDotLine,
        DashDotDotLine
    };

    Q_ENUM(GLColorType)
    Q_ENUM(GLSeriesType)
    Q_ENUM(GLMarkerShape)
    Q_ENUM(GLAutoScalePolicy)
    Q_ENUM(GLItemType)
    Q_ENUM(GLStyleLine)
};

Q_DECLARE_METATYPE(GLSeriesType) // Decalartion reason is: this enum want use as type of Q_PROPERTY(...).
Q_DECLARE_METATYPE(GLColorType) // Decalartion reason is: this enum want use as type of Q_PROPERTY(...).
Q_DECLARE_METATYPE(GLMarkerShape) // Decalartion reason is: this enum want use as type of Q_PROPERTY(...).
Q_DECLARE_METATYPE(GLAutoScalePolicy) // Decalartion reason is: this enum want use as type of Q_PROPERTY(...).
Q_DECLARE_METATYPE(GLItemType) // Decalartion reason is: this enum want use as type of Q_PROPERTY(...).
Q_DECLARE_METATYPE(GLStyleLine) // Decalartion reason is: this enum want use as type of Q_PROPERTY(...).

#endif // GLENUMS_H
