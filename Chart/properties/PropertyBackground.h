#ifndef PROPERTYBACKGROUND_H
#define PROPERTYBACKGROUND_H

#include "IProperties.h"

class PropertyBackground : public IProperties {
    Q_OBJECT
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY changed)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY changed)
public:
    PropertyBackground(QQuickItem *parent = nullptr) : IProperties{parent} {}

    const QString color() const {
        return m_color;
    }
    void setColor(const QString &pColor) {
        m_color = pColor;
        emit changed();
    }

    qreal opacity() const {
        return m_opacity;
    }
    void setOpacity(const qreal &pOpacity) {
        m_opacity = pOpacity;
        emit changed();
    }

private:
    QString m_color = "#f9f9f9";
    qreal m_opacity = 7.0;
};


#endif
