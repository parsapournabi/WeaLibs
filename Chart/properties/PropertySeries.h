#ifndef PROPERTYSERIES_H
#define PROPERTYSERIES_H

#include "IProperties.h"
#include "GLEnums.h"
#include <QColor>
#include <QDebug>
#include <QImage>

class PropertySeries : public IProperties {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(GLColorType colorType READ colorType WRITE setColorType NOTIFY colorTypeChanged)
    Q_PROPERTY(GLSeriesType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(GLMarkerShape markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged) // only at scatter
    Q_PROPERTY(int markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged) // only at scatter
//    Q_PROPERTY(QImage markerIcon READ markerIcon NOTIFY markerIconChanged)
    Q_PROPERTY(QString markerIconUrl READ markerIconUrl WRITE setMarkerIconUrl NOTIFY markerIconUrlChanged)
public:
    explicit PropertySeries(QQuickItem *parent = nullptr) : IProperties{parent} {}

    inline QString name() const { return m_name; }
    inline void setName(const QString &name) {
        if (m_name == name) return;
        m_name = name;
        emit nameChanged();
        emit changed();
    }

    inline bool visible() const { return m_visible; }
    inline void setVisible(bool visible) {
        if (m_visible == visible) return;
        m_visible = visible;
        emit visibleChanged();
        emit changed();
    }

    inline QColor color() const { return m_color; }
    inline void setColor(const QColor &color) {
        if (m_color == color) return;
        m_color = color;
        emit colorChanged();
        emit changed();
    }

    inline GLColorType colorType() const { return m_colorType; }
    inline void setColorType(GLColorType type) {
        if (m_colorType == type) return;
        m_colorType = type;
        emit colorTypeChanged();
        emit changed();
    }

    inline GLSeriesType type() const { return m_type; }
    inline void setType(GLSeriesType type) {
        if (m_type == type) return;
        m_type = type;
        emit typeChanged();
        emit changed();
    }
    inline void setType(int type) {
        if (m_type == type) return;
        m_type = static_cast<GLSeriesType>(type);
        emit typeChanged();
        emit changed();
    }

    inline GLMarkerShape markerShape() const { return m_markerShape; }
    inline void setMarkerShape(GLMarkerShape shape) {
        if (m_markerShape == shape) return;
        m_markerShape = shape;
        emit markerShapeChanged();
        emit changed();
    }

    inline int markerSize() const { return m_markerSize; }
    inline void setMarkerSize(int size) {
        if (m_markerSize == size) return;
        m_markerSize = size;
        emit markerSizeChanged();
        emit changed();
    }

    inline QImage *markerIconPtr() const { return &m_markerIcon; }
    inline const QImage &markerIcon() const { return m_markerIcon; }
    inline void setMarkerIcon(const QString &path) {
        m_markerIconUrl = path;
        m_markerIcon = QImage(m_markerIconUrl);

//        emit markerIconChanged();
        emit changed();
    }
    inline QString markerIconUrl() const { return m_markerIconUrl; }
    inline void setMarkerIconUrl(QString url) {
        if (url.startsWith("qrc:")) url.replace(0, 3, "");
        if (m_markerIconUrl == url) return;
        m_markerIconUrl = url;

        emit markerIconUrlChanged();
//        emit changed();

        // Auto Creating new Image
        setMarkerIcon(m_markerIconUrl);
    }

signals:
    void nameChanged();
//    void visibleChanged();
    void colorChanged();
    void colorTypeChanged();
    void typeChanged();
    void markerShapeChanged();
    void markerSizeChanged();
//    void markerIconChanged();
    void markerIconUrlChanged();
    void selected(const QVariant &payload, int vectorTypeId);

protected:
    QString m_name = "Unknown series name";
    bool m_visible = true;
    QColor m_color;
    GLColorType m_colorType = GLColorType::MixBasePointColor;
    GLSeriesType m_type = GLSeriesType::SeriesTypeScatter;
    GLMarkerShape m_markerShape = GLMarkerShape::ShapeSquare;
    int m_markerSize = 10;
    mutable QImage m_markerIcon;
    QString m_markerIconUrl;
};

#endif // PROPERTYSERIES_H
