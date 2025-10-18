#ifndef GLCHARTLINEITEM_H
#define GLCHARTLINEITEM_H

#include "WeaChart/overlay/tools/GLChartItemBase.h"

/// @brief An AbstractChartLineItem inherits from GLChartItemBase and with more line options.
class WEACHART_API GLChartLineItem : public GLChartItemBase
{
    Q_OBJECT
    /// @brief number of the lineWidth ChartItem.
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    /// @brief position of the ChartItem.
    /// @note the position if vertical is true will modify "x", else will modify "y" axes.
    Q_PROPERTY(qreal position READ position WRITE setPosition NOTIFY positionChanged)
public:
    explicit GLChartLineItem(QQuickItem *parent = nullptr);

    /// @brief getter of the lineWidth property.
    /// @return qreal.
    qreal lineWidth() const;
    /// @brief setter of the lineWidth property.
    /// @param qreal.
    void setLineWidth(const qreal &width);

    /// @brief getter of the position property.
    /// @return qreal.
    qreal position() const;
    /// @brief setter of the position property.
    /// @param qreal.
    void setPosition(const qreal &pos);

signals:
    void lineWidthChanged();
    void positionChanged();
protected:
    QVector<QVector2D> vertices() const noexcept override;
    GLItemType type() const override;
    GLenum drawType() const override;

private:
    qreal m_lineWidth = 1.0;
    qreal m_position;
};

#endif // GLCHARTLINEITEM_H
