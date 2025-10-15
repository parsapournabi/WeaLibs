#ifndef GLCHARTLINEITEM_H
#define GLCHARTLINEITEM_H

#include "WeaChart/overlay/tools/GLChartItemBase.h"

class GLChartLineItem : public GLChartItemBase
{
    Q_OBJECT
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(qreal position READ position WRITE setPosition NOTIFY positionChanged)
public:
    explicit GLChartLineItem(QQuickItem *parent = nullptr);

    qreal lineWidth() const;
    void setLineWidth(const qreal &width);

    qreal position() const;
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
