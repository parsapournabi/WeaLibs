#ifndef PROPERTYSERIES_H
#define PROPERTYSERIES_H

#include "WeaChart/series/GLEnums.h"
#include "IProperties.h"

#include <QColor>
#include <QDebug>
#include <QImage>

/// @brief Abstract Series properties contains in this class.
class WEACHART_API PropertySeries : public IProperties {
    Q_OBJECT
    /// @brief name of the series which uses on GLLegend (UI).
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    /// @brief Series visibility, making "false" of this property will ignore series process & UI (also it will hide at GLLegend).
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    /// @details: color: QColor => base color of series,
    ///  @note series contains at least Two type of colors (referer `colorType`,
    /// color prop is base color of series which can MIX with each "points color" or "only base color" or ignoring base color (See colorType property.)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    /// @brief The type of series color for showing at UI.
    ///  @note please see "WeaChart/series/GLEnums.h" for more details.
    Q_PROPERTY(GLColorType colorType READ colorType WRITE setColorType NOTIFY colorTypeChanged)
    /// @brief There is three type of series, "scatter", "line" and "area",
    ///  @note please see "WeaChart/series/GLEnums.h" for more details.
    Q_PROPERTY(GLSeriesType type READ type WRITE setType NOTIFY typeChanged)
    /// @brief The shape of marker (ONLY ON SCATTER TYPE) visual.
    ///  @note please see "WeaChart/series/GLEnums.h" for more details.
    Q_PROPERTY(GLMarkerShape markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged) // only at scatter
    /// @brief Only in the scatterSeriesType, The size of the points.
    Q_PROPERTY(int markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged) // only at scatter
    /// @brief When you choose `GLMarkerShape::ShapeTexture`, you must set this property to :/path/of/your/icon.
    Q_PROPERTY(QString markerIconUrl READ markerIconUrl WRITE setMarkerIconUrl NOTIFY markerIconUrlChanged)
    /// @brief Make this property true if the buffer X axis is sorted.
    Q_PROPERTY(bool seriesXSorted READ seriesXSorted WRITE setSeriesXSorted NOTIFY seriesXSortedChanged)
    /// @brief Make this property true if the buffer Y axis is sorted.
    Q_PROPERTY(bool seriesYSorted READ seriesYSorted WRITE setSeriesYSorted NOTIFY seriesYSortedChanged)
public:
    explicit PropertySeries(QQuickItem *parent = nullptr) : IProperties{parent} {}

    /// @brief getter of the name property.
    inline QString name() const { return m_name; }
    /// @brief setter of the name property.
    inline void setName(const QString &name) {
        if (m_name == name) return;
        m_name = name;
        emit nameChanged();
        emit changed();
    }

    /// @brief getter of the visible property.
    inline bool visible() const { return m_visible; }
    /// @brief setter of the visible property.
    inline void setVisible(bool visible) {
        if (m_visible == visible) return;
        m_visible = visible;
        emit visibleChanged();
        emit changed();
    }

    /// @brief getter of the color property.
    inline QColor color() const { return m_color; }
    /// @brief setter of the color property.
    inline void setColor(const QColor &color) {
        if (m_color == color) return;
        m_color = color;
        emit colorChanged();
        emit changed();
    }

    /// @brief getter of the colorType property.
    inline GLColorType colorType() const { return m_colorType; }
    /// @brief setter of the colorType property.
    inline void setColorType(GLColorType type) {
        if (m_colorType == type) return;
        m_colorType = type;
        emit colorTypeChanged();
        emit changed();
    }

    /// @brief getter of the type property.
    inline GLSeriesType type() const { return m_type; }
    /// @brief setter of the type property.
    inline void setType(GLSeriesType type) {
        if (m_type == type) return;
        m_type = type;
        emit typeChanged();
        emit changed();
    }
    /// @brief setter of the type property.
    inline void setType(int type) {
        if (m_type == type) return;
        m_type = static_cast<GLSeriesType>(type);
        emit typeChanged();
        emit changed();
    }

    /// @brief getter of the markerShape property.
    inline GLMarkerShape markerShape() const { return m_markerShape; }
    /// @brief setter of the markerShape property.
    inline void setMarkerShape(GLMarkerShape shape) {
        if (m_markerShape == shape) return;
        m_markerShape = shape;
        emit markerShapeChanged();
        emit changed();
    }

    /// @brief getter of the markerSize property.
    inline int markerSize() const { return m_markerSize; }
    /// @brief setter of the markerSize property.
    inline void setMarkerSize(int size) {
        if (m_markerSize == size) return;
        m_markerSize = size;
        emit markerSizeChanged();
        emit changed();
    }

    /// @return markerIcon as an image.
    inline QImage *markerIconPtr() const { return &m_markerIcon; }
    /// @brief getter of the markerIcon private property.
    inline const QImage &markerIcon() const { return m_markerIcon; }
    /// @brief setter of the markerIcon private property.
    inline void setMarkerIcon(const QString &path) {
        m_markerIconUrl = path;
        m_markerIcon = QImage(m_markerIconUrl);

//        emit markerIconChanged();
        emit changed();
    }
    /// @brief getter of the markerIconUrl property.
    inline QString markerIconUrl() const { return m_markerIconUrl; }
    /// @brief setter of the markerIconUrl property.
    inline void setMarkerIconUrl(QString url) {
        if (url.startsWith("qrc:")) url.replace(0, 3, "");
        if (m_markerIconUrl == url) return;
        m_markerIconUrl = url;

        emit markerIconUrlChanged();
//        emit changed();

        // Auto Creating new Image
        setMarkerIcon(m_markerIconUrl);
    }

    /// @brief sync with seriesXSorted property.
    inline bool isXSorted() const noexcept { return m_seriesXSorted; }
    /// @brief getter of the seriesXSorted property.
    inline bool seriesXSorted() const noexcept { return m_seriesXSorted; }
    /// @brief setter of the seriesXSorted property.
    inline void setSeriesXSorted(bool sorted) {
        if (sorted == m_seriesXSorted) return;
        m_seriesXSorted = sorted;
        emit seriesXSortedChanged();
        emit changed();
    }
    /// @brief sync with seriesYSorted property.
    inline bool isYSorted() const noexcept { return m_seriesYSorted; }
    /// @brief getter of the seriesYSorted property.
    inline bool seriesYSorted() const noexcept { return m_seriesYSorted; }
    /// @brief setter of the seriesYSorted property.
    inline void setSeriesYSorted(bool sorted) {
        if (sorted == m_seriesYSorted) return;
        m_seriesYSorted = sorted;
        emit seriesYSortedChanged();
        emit changed();
    }

signals:
    void nameChanged();
    void colorChanged();
    void colorTypeChanged();
    void typeChanged();
    void markerShapeChanged();
    void markerSizeChanged();
    void markerIconUrlChanged();
    void seriesXSortedChanged();
    void seriesYSortedChanged();

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
    bool m_seriesXSorted = false;
    bool m_seriesYSorted = false;
};

#endif // PROPERTYSERIES_H
