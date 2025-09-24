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
    Q_ENUM(GLColorType)
    Q_ENUM(GLSeriesType)
    Q_ENUM(GLMarkerShape)
};

Q_DECLARE_METATYPE(GLSeriesType) // Decalartion reason is: this enum want use as type of Q_PROPERTY(...).
Q_DECLARE_METATYPE(GLColorType) // Decalartion reason is: this enum want use as type of Q_PROPERTY(...).
Q_DECLARE_METATYPE(GLMarkerShape) // Decalartion reason is: this enum want use as type of Q_PROPERTY(...).

#endif // GLENUMS_H
