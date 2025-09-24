#ifndef PROPERTYAXISRANGE_H
#define PROPERTYAXISRANGE_H

#include "IProperties.h"

/// @brief Contains the minX, maxX, minY, maxY properties, which define the limits of the Chart XY axes.
/// @details PropertyAxis Range will use on `GLChartView` class property.
/// This is much cleaner than using 4 properties(minX, maxX, minY and ...) instead of a single property
/// at `GLChartView` class.
class WEACHART_API PropertyAxisRange : public IProperties {
    Q_OBJECT
    /// @brief Minimum number chart X axe.(left)
    Q_PROPERTY(qreal minX READ minX WRITE setMinX NOTIFY changed)
    /// @brief Minimum number chart Y axe.(bottom)
    Q_PROPERTY(qreal minY READ minY WRITE setMinY NOTIFY changed)
    /// @brief Maximum number chart X axe.(right)
    Q_PROPERTY(qreal maxX READ maxX WRITE setMaxX NOTIFY changed)
    /// @brief Maximum number chart Y axe.(top)
    Q_PROPERTY(qreal maxY READ maxY WRITE setMaxY NOTIFY changed)
public:
    PropertyAxisRange(QQuickItem *parent = nullptr) : IProperties{parent} {}

    /// @brief getter of the minX property.
    const qreal &minX() const {
        return m_minX;
    }
    /// @brief setter of the minX property.
    void setMinX(const qreal &pMinX) {
        m_minX = pMinX;
        emit changed();
    }
    /// @brief getter of the maxX property.
    const qreal &maxX() const {
        return m_maxX;
    }
    /// @brief setter of the maxX property.
    void setMaxX(const qreal &pMaxX) {
        m_maxX = pMaxX;
        emit changed();
    }

    /// @brief getter of the minY property.
    const qreal &minY() const {
        return m_minY;
    }
    /// @brief setter of the minY property.
    void setMinY(const qreal &pMinY) {
        m_minY = pMinY;
        emit changed();
    }
    /// @brief getter of the maxY property.
    const qreal &maxY() const {
        return m_maxY;
    }
    /// @brief setter of the maxY property.
    void setMaxY(const qreal &pMaxY) {
        m_maxY = pMaxY;
        emit changed();
    }

private:
    /// @brief Member of the minX property.
    qreal m_minX = 0.0;
    /// @brief Member of the maxX property.
    qreal m_maxX = 100.0;
    /// @brief Member of the minY property.
    qreal m_minY = 0.0;
    /// @brief Member of the maxY property.
    qreal m_maxY = 100.0;

};

#endif // PROPERTYAXISRANGE_H
