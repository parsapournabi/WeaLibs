#ifndef GLCHARTGATEITEM_H
#define GLCHARTGATEITEM_H

#include "GLChartItemBase.h"

/// @brief An AbstractChartGateItem inherits from GLChartItemBase and with more gate options.
class WEACHART_API GLChartGateItem : public GLChartItemBase
{
    Q_OBJECT
    /// @brief the first point position. (If the vertical is true it reference with "x", else its reference is "y" axes).
    Q_PROPERTY(qreal from READ from WRITE setFrom NOTIFY fromChanged)
    /// @brief the second point position. (If the vertical is true it reference with "x", else its reference is "y" axes).
    Q_PROPERTY(qreal to READ to WRITE setTo NOTIFY toChanged)
public:
    explicit GLChartGateItem(QQuickItem *parent = nullptr);

    /// @brief getter of the from property.
    /// @return qreal.
    qreal from() const;
    /// @brief setter of the from property.
    /// @param qreal.
    void setFrom(const qreal &pos);

    /// @brief getter of the to property.
    /// @return qreal.
    qreal to() const;
    /// @brief setter of the to property.
    /// @param qreal.
    void setTo(const qreal &pos);

signals:
    void fromChanged();
    void toChanged();
protected:
    QVector<QVector2D> vertices() const noexcept override;
    GLItemType type() const override;
    GLenum drawType() const override;

private:
    qreal m_from;
    qreal m_to;

};

#endif // GLCHARTGATEITEM_H
