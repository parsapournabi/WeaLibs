#ifndef PROPERTYBACKGROUND_H
#define PROPERTYBACKGROUND_H

#include "IProperties.h"

/// @brief the color/opacity style of OpenGL FramebufferObject (FBO)
class WEACHART_API PropertyBackground : public IProperties {
    Q_OBJECT
    /// @brief Background color of the FBO.
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY changed)
    /// @brief Background opacity of the FBO.
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY changed)
public:
    PropertyBackground(QQuickItem *parent = nullptr) : IProperties{parent} {}

    /// @brief getter of the color property.
    const QString color() const {
        return m_color;
    }
    /// @brief setter of the color property.
    void setColor(const QString &pColor) {
        m_color = pColor;
        emit changed();
    }

    /// @brief getter of the opacity property.
    qreal opacity() const {
        return m_opacity;
    }
    /// @brief setter of the opacity property.
    void setOpacity(const qreal &pOpacity) {
        m_opacity = pOpacity;
        emit changed();
    }

private:
    /// @brief Member of the color property.
    QString m_color = "#f9f9f9";
    /// @brief Member of the opacity property.
    qreal m_opacity = 7.0;
};


#endif
