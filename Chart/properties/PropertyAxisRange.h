#ifndef PROPERTYAXISRANGE_H
#define PROPERTYAXISRANGE_H

#include "IProperties.h"

class PropertyAxisRange : public IProperties {
    Q_OBJECT
    Q_PROPERTY(qreal minX READ minX WRITE setMinX NOTIFY changed)
    Q_PROPERTY(qreal minY READ minY WRITE setMinY NOTIFY changed)
    Q_PROPERTY(qreal maxX READ maxX WRITE setMaxX NOTIFY changed)
    Q_PROPERTY(qreal maxY READ maxY WRITE setMaxY NOTIFY changed)
public:
    PropertyAxisRange(QObject *parent = nullptr) : IProperties{parent} {}

    const qreal &minX() const {
        return m_minX;
    }
    void setMinX(const qreal &pMinX) {
        m_minX = pMinX;
        emit changed();
    }
    const qreal &maxX() const {
        return m_maxX;
    }
    void setMaxX(const qreal &pMaxX) {
        m_maxX = pMaxX;
        emit changed();
    }

    const qreal &minY() const {
        return m_minY;
    }
    void setMinY(const qreal &pMinY) {
        m_minY = pMinY;
        emit changed();
    }
    const qreal &maxY() const {
        return m_maxY;
    }
    void setMaxY(const qreal &pMaxY) {
        m_maxY = pMaxY;
        emit changed();
    }

private:
    qreal m_minX = 0.0;
    qreal m_maxX = 100.0;
    qreal m_minY = 0.0;
    qreal m_maxY = 100.0;

};

#endif // PROPERTYAXISRANGE_H
