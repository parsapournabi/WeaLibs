#ifndef GLENUMS_H
#define GLENUMS_H

#include <QObject>

enum GLSeriesType {
    SeriesTypeScatter = 0, // 0
    SeriesTypeLine,
    SeriesTypeArea,
};
Q_DECLARE_METATYPE(GLSeriesType)

enum GLColorType {
    MixBasePointColor = 0, // 0
    OnlyBaseColor,
    OnlyPointColor
};
Q_DECLARE_METATYPE(GLColorType)

enum GLMarkerShape {
    ShapeSquare = 0, // 0
    ShapeCircle,
    ShapeTexture
};
Q_DECLARE_METATYPE(GLMarkerShape)

class GLMetaEnums : public QObject {
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

#endif // GLENUMS_H
